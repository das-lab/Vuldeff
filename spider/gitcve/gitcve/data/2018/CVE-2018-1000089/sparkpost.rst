.. _sparkpost-backend:

SparkPost
=========

Anymail integrates with the `SparkPost`_ email service, using their
:pypi:`python-sparkpost` API client.

.. _SparkPost: https://www.sparkpost.com/


Installation
------------

You must ensure the :pypi:`sparkpost` package is installed to use Anymail's SparkPost
backend. Either include the "sparkpost" option when you install Anymail:

    .. code-block:: console

        $ pip install django-anymail[sparkpost]

or separately run `pip install sparkpost`.


Settings
--------


.. rubric:: EMAIL_BACKEND

To use Anymail's SparkPost backend, set:

  .. code-block:: python

      EMAIL_BACKEND = "anymail.backends.sparkpost.EmailBackend"

in your settings.py.


.. setting:: ANYMAIL_SPARKPOST_API_KEY

.. rubric:: SPARKPOST_API_KEY

A SparkPost API key with at least the "Transmissions: Read/Write" permission.
(Manage API keys in your `SparkPost account API keys`_.)

This setting is optional; if not provided, the SparkPost API client will attempt
to read your API key from the `SPARKPOST_API_KEY` environment variable.

  .. code-block:: python

      ANYMAIL = {
          ...
          "SPARKPOST_API_KEY": "<your API key>",
      }

Anymail will also look for ``SPARKPOST_API_KEY`` at the
root of the settings file if neither ``ANYMAIL["SPARKPOST_API_KEY"]``
nor ``ANYMAIL_SPARKPOST_API_KEY`` is set.

.. _SparkPost account API keys: https://app.sparkpost.com/account/credentials


.. _sparkpost-esp-extra:

esp_extra support
-----------------

To use SparkPost features not directly supported by Anymail, you can
set a message's :attr:`~anymail.message.AnymailMessage.esp_extra` to
a `dict` of parameters for python-sparkpost's `transmissions.send method`_.
Any keys in your :attr:`esp_extra` dict will override Anymail's normal
values for that parameter.

Example:

    .. code-block:: python

        message.esp_extra = {
            'transactional': True,  # treat as transactional for unsubscribe and suppression
            'description': "Marketing test-run for new templates",
            'use_draft_template': True,
        }


(You can also set `"esp_extra"` in Anymail's :ref:`global send defaults <send-defaults>`
to apply it to all messages.)

.. _transmissions.send method:
    https://python-sparkpost.readthedocs.io/en/latest/api/transmissions.html#sparkpost.transmissions.Transmissions.send



Limitations and quirks
----------------------

.. _sparkpost-message-id:

**Anymail's `message_id` is SparkPost's `transmission_id`**
  The :attr:`~anymail.message.AnymailStatus.message_id` Anymail sets
  on a message's :attr:`~anymail.message.AnymailMessage.anymail_status`
  and in normalized webhook :class:`~anymail.signals.AnymailTrackingEvent`
  data is actually what SparkPost calls "transmission_id".

  Like Anymail's message_id for other ESPs, SparkPost's transmission_id
  (together with the recipient email address), uniquely identifies a
  particular message instance in tracking events.

  (The transmission_id is the only unique identifier available when you
  send your message. SparkPost also has something called "message_id", but
  that doesn't get assigned until after the send API call has completed.)

  If you are working exclusively with Anymail's normalized message status
  and webhook events, the distinction won't matter: you can consistently
  use Anymail's `message_id`. But if you are also working with raw webhook
  esp_event data or SparkPost's events API, be sure to think "transmission_id"
  wherever you're speaking to SparkPost.

**Single tag**
  Anymail uses SparkPost's "campaign_id" to implement message tagging.
  SparkPost only allows a single campaign_id per message. If your message has
  two or more :attr:`~anymail.message.AnymailMessage.tags`, you'll get an
  :exc:`~anymail.exceptions.AnymailUnsupportedFeature` error---or
  if you've enabled :setting:`ANYMAIL_IGNORE_UNSUPPORTED_FEATURES`,
  Anymail will use only the first tag.

  (SparkPost's "recipient tags" are not available for tagging *messages*.
  They're associated with individual *addresses* in stored recipient lists.)


.. _sparkpost-templates:

Batch sending/merge and ESP templates
-------------------------------------

SparkPost offers both :ref:`ESP stored templates <esp-stored-templates>`
and :ref:`batch sending <batch-send>` with per-recipient merge data.

You can use a SparkPost stored template by setting a message's
:attr:`~anymail.message.AnymailMessage.template_id` to the
template's unique id. (When using a stored template, SparkPost prohibits
setting the EmailMessage's subject, text body, or html body.)

Alternatively, you can refer to merge fields directly in an EmailMessage's
subject, body, and other fields---the message itself is used as an
on-the-fly template.

In either case, supply the merge data values with Anymail's
normalized :attr:`~anymail.message.AnymailMessage.merge_data`
and :attr:`~anymail.message.AnymailMessage.merge_global_data`
message attributes.

  .. code-block:: python

      message = EmailMessage(
          ...
          to=["alice@example.com", "Bob <bob@example.com>"]
      )
      message.template_id = "11806290401558530"  # SparkPost id
      message.merge_data = {
          'alice@example.com': {'name': "Alice", 'order_no': "12345"},
          'bob@example.com': {'name': "Bob", 'order_no': "54321"},
      }
      message.merge_global_data = {
          'ship_date': "May 15",
          # Can use SparkPost's special "dynamic" keys for nested substitutions (see notes):
          'dynamic_html': {
              'status_html': "<a href='https://example.com/order/{{order_no}}'>Status</a>",
          },
          'dynamic_plain': {
              'status_plain': "Status: https://example.com/order/{{order_no}}",
          },
      }


See `SparkPost's substitutions reference`_ for more information on templates and
batch send with SparkPost. If you need the special `"dynamic" keys for nested substitutions`_,
provide them in Anymail's :attr:`~anymail.message.AnymailMessage.merge_global_data`
as shown in the example above. And if you want `use_draft_template` behavior, specify that
in :ref:`esp_extra <sparkpost-esp-extra>`.


.. _SparkPost's substitutions reference:
    https://developers.sparkpost.com/api/substitutions-reference

.. _"dynamic" keys for nested substitutions:
    https://developers.sparkpost.com/api/substitutions-reference#header-links-and-substitution-expressions-within-substitution-values


.. _sparkpost-webhooks:

Status tracking webhooks
------------------------

If you are using Anymail's normalized :ref:`status tracking <event-tracking>`, set up the
webhook in your `SparkPost account settings under "Webhooks"`_:

* Target URL: :samp:`https://{yoursite.example.com}/anymail/sparkpost/tracking/`
* Authentication: choose "Basic Auth." For username and password enter the two halves of the
  *random:random* shared secret you created for your :setting:`ANYMAIL_WEBHOOK_AUTHORIZATION`
  Django setting. (Anymail doesn't support OAuth webhook auth.)
* Events: click "Select" and then *clear* the checkbox for "Relay Events" category (which is for
  inbound email). You can leave all the other categories of events checked, or disable
  any you aren't interested in tracking.

SparkPost will report these Anymail :attr:`~anymail.signals.AnymailTrackingEvent.event_type`\s:
queued, rejected, bounced, deferred, delivered, opened, clicked, complained, unsubscribed,
subscribed.

The event's :attr:`~anymail.signals.AnymailTrackingEvent.esp_event` field will be
a single, raw `SparkPost event`_. (Although SparkPost calls webhooks with batches of events,
Anymail will invoke your signal receiver separately for each event in the batch.)
The esp_event is the raw, `wrapped json event structure`_ as provided by SparkPost:
`{'msys': {'<event_category>': {...<actual event data>...}}}`.


.. _SparkPost account settings under "Webhooks":
    https://app.sparkpost.com/account/webhooks
.. _SparkPost event:
    https://support.sparkpost.com/customer/portal/articles/1976204-webhook-event-reference
.. _wrapped json event structure:
    https://support.sparkpost.com/customer/en/portal/articles/2311698-comparing-webhook-and-message-event-data


.. _sparkpost-inbound:

Inbound webhook
---------------

If you want to receive email from SparkPost through Anymail's normalized :ref:`inbound <inbound>`
handling, follow SparkPost's `Enabling Inbound Email Relaying`_ guide to set up
Anymail's inbound webhook.

The target parameter for the Relay Webhook will be:

   :samp:`https://{random}:{random}@{yoursite.example.com}/anymail/sparkpost/inbound/`

     * *random:random* is an :setting:`ANYMAIL_WEBHOOK_AUTHORIZATION` shared secret
     * *yoursite.example.com* is your Django site

.. _Enabling Inbound Email Relaying:
   https://www.sparkpost.com/docs/tech-resources/inbound-email-relay-webhook/

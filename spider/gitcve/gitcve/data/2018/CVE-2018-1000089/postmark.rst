.. _postmark-backend:

Postmark
========

Anymail integrates with the `Postmark`_ transactional email service,
using their `HTTP email API`_.

.. _Postmark: https://postmarkapp.com/
.. _HTTP email API: http://developer.postmarkapp.com/developer-api-email.html


Settings
--------

.. rubric:: EMAIL_BACKEND

To use Anymail's Postmark backend, set:

  .. code-block:: python

      EMAIL_BACKEND = "anymail.backends.postmark.EmailBackend"

in your settings.py.


.. setting:: ANYMAIL_POSTMARK_SERVER_TOKEN

.. rubric:: POSTMARK_SERVER_TOKEN

Required. A Postmark server token.

  .. code-block:: python

      ANYMAIL = {
          ...
          "POSTMARK_SERVER_TOKEN": "<your server token>",
      }

Anymail will also look for ``POSTMARK_SERVER_TOKEN`` at the
root of the settings file if neither ``ANYMAIL["POSTMARK_SERVER_TOKEN"]``
nor ``ANYMAIL_POSTMARK_SERVER_TOKEN`` is set.

You can override the server token for an individual message in
its :ref:`esp_extra <postmark-esp-extra>`.


.. setting:: ANYMAIL_POSTMARK_API_URL

.. rubric:: POSTMARK_API_URL

The base url for calling the Postmark API.

The default is ``POSTMARK_API_URL = "https://api.postmarkapp.com/"``
(It's unlikely you would need to change this.)


.. _postmark-esp-extra:

esp_extra support
-----------------

To use Postmark features not directly supported by Anymail, you can
set a message's :attr:`~anymail.message.AnymailMessage.esp_extra` to
a `dict` that will be merged into the json sent to Postmark's
`email API`_.

Example:

    .. code-block:: python

        message.esp_extra = {
            'HypotheticalFuturePostmarkParam': '2022',  # merged into send params
            'server_token': '<API server token for just this message>',
        }


(You can also set `"esp_extra"` in Anymail's
:ref:`global send defaults <send-defaults>` to apply it to all
messages.)


.. _email API: http://developer.postmarkapp.com/developer-api-email.html


Limitations and quirks
----------------------

Postmark does not support a few tracking and reporting additions offered by other ESPs.

Anymail normally raises an :exc:`~anymail.exceptions.AnymailUnsupportedFeature`
error when you try to send a message using features that Postmark doesn't support
You can tell Anymail to suppress these errors and send the messages anyway --
see :ref:`unsupported-features`.

**Single tag**
  Postmark allows a maximum of one tag per message. If your message has two or more
  :attr:`~anymail.message.AnymailMessage.tags`, you'll get an
  :exc:`~anymail.exceptions.AnymailUnsupportedFeature` error---or
  if you've enabled :setting:`ANYMAIL_IGNORE_UNSUPPORTED_FEATURES`,
  Anymail will use only the first tag.

**No metadata**
  Postmark does not support attaching :attr:`~anymail.message.AnymailMessage.metadata`
  to messages.

**No delayed sending**
  Postmark does not support :attr:`~anymail.message.AnymailMessage.send_at`.

**Click-tracking**
  Postmark supports `several link-tracking options`_. Anymail treats
  :attr:`~anymail.message.AnymailMessage.track_clicks` as Postmark's
  "HtmlAndText" option when True.

  If you would prefer Postmark's "HtmlOnly" or "TextOnly" link-tracking, you could
  either set that as a Postmark server-level default (and use `message.track_clicks = False`
  to disable tracking for specific messages), or use something like
  `message.esp_extra = {'TrackLinks': "HtmlOnly"}` to specify a particular option.

.. _several link-tracking options:
   http://developer.postmarkapp.com/developer-link-tracking.html


.. _postmark-templates:

Batch sending/merge and ESP templates
-------------------------------------

Postmark supports :ref:`ESP stored templates <esp-stored-templates>`
populated with global merge data for all recipients, but does not
offer :ref:`batch sending <batch-send>` with per-recipient merge data.
Anymail's :attr:`~anymail.message.AnymailMessage.merge_data`
message attribute is not supported with the Postmark backend.

To use a Postmark template, set the message's
:attr:`~anymail.message.AnymailMessage.template_id` to the numeric
Postmark "TemplateID" and supply the "TemplateModel" using
the :attr:`~anymail.message.AnymailMessage.merge_global_data`
message attribute:

  .. code-block:: python

      message = EmailMessage(
          ...
          subject=None,  # use template subject
          to=["alice@example.com"]  # single recipient...
          # ...multiple to emails would all get the same message
          # (and would all see each other's emails in the "to" header)
      )
      message.template_id = 80801  # use this Postmark template
      message.merge_global_data = {
          'name': "Alice",
          'order_no': "12345",
          'ship_date': "May 15",
          'items': [
              {'product': "Widget", 'price': "9.99"},
              {'product': "Gadget", 'price': "17.99"},
          ],
      }

Set the EmailMessage's subject to `None` to use the subject from
your Postmark template, or supply a subject with the message to override
the template value.

See this `Postmark blog post on templates`_ for more information.

.. _Postmark blog post on templates:
    https://postmarkapp.com/blog/special-delivery-postmark-templates


.. _postmark-webhooks:

Status tracking webhooks
------------------------

If you are using Anymail's normalized :ref:`status tracking <event-tracking>`, enter
the url in your `Postmark account settings`_, under Servers > *your server name* >
Settings > Outbound > Webhooks. You should enter this same Anymail tracking URL
for all of the "Delivery webhook," "Bounce webhook," and "Opens webhook" (if you
want to receive all these types of events):

   :samp:`https://{random}:{random}@{yoursite.example.com}/anymail/postmark/tracking/`

     * *random:random* is an :setting:`ANYMAIL_WEBHOOK_AUTHORIZATION` shared secret
     * *yoursite.example.com* is your Django site

Anymail doesn't care about the "include bounce content" and "post only on first open"
Postmark webhook settings: whether to use them is your choice.

If you use multiple Postmark servers, you'll need to repeat entering the webhook
settings for each of them.

Postmark will report these Anymail :attr:`~anymail.signals.AnymailTrackingEvent.event_type`\s:
rejected, failed, bounced, deferred, delivered, autoresponded, opened, clicked, complained,
unsubscribed, subscribed. (Postmark does not support sent--what it calls "processed"--events
through webhooks.)

The event's :attr:`~anymail.signals.AnymailTrackingEvent.esp_event` field will be
a `dict` of Postmark `delivery <http://developer.postmarkapp.com/developer-delivery-webhook.html>`_,
`bounce <http://developer.postmarkapp.com/developer-bounce-webhook.html>`_,
or `open <http://developer.postmarkapp.com/developer-open-webhook.html>`_ webhook data.

.. _Postmark account settings: https://account.postmarkapp.com/servers


.. _postmark-inbound:

Inbound webhook
---------------

If you want to receive email from Postmark through Anymail's normalized :ref:`inbound <inbound>`
handling, follow Postmark's `Inbound Processing`_ guide to configure
an inbound server pointing to Anymail's inbound webhook.

The InboundHookUrl setting will be:

   :samp:`https://{random}:{random}@{yoursite.example.com}/anymail/postmark/inbound/`

     * *random:random* is an :setting:`ANYMAIL_WEBHOOK_AUTHORIZATION` shared secret
     * *yoursite.example.com* is your Django site

Anymail handles the "parse an email" part of Postmark's instructions for you, but you'll
likely want to work through the other sections to set up a custom inbound domain, and
perhaps configure inbound spam blocking.

.. _Inbound Processing: https://postmarkapp.com/developer/user-guide/inbound

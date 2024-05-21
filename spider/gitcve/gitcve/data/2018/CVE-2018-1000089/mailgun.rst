.. _mailgun-backend:

Mailgun
=======

Anymail integrates with the `Mailgun <https://mailgun.com>`_
transactional email service from Rackspace, using their
REST API.


Settings
--------

.. rubric:: EMAIL_BACKEND

To use Anymail's Mailgun backend, set:

  .. code-block:: python

      EMAIL_BACKEND = "anymail.backends.mailgun.EmailBackend"

in your settings.py.


.. setting:: ANYMAIL_MAILGUN_API_KEY

.. rubric:: MAILGUN_API_KEY

Required. Your Mailgun API key:

  .. code-block:: python

      ANYMAIL = {
          ...
          "MAILGUN_API_KEY": "<your API key>",
      }

Anymail will also look for ``MAILGUN_API_KEY`` at the
root of the settings file if neither ``ANYMAIL["MAILGUN_API_KEY"]``
nor ``ANYMAIL_MAILGUN_API_KEY`` is set.


.. setting:: ANYMAIL_MAILGUN_SENDER_DOMAIN

.. rubric:: MAILGUN_SENDER_DOMAIN

If you are using a specific `Mailgun sender domain`_
that is *different* from your messages' `from_email` domains,
set this to the domain you've configured in your Mailgun account.

If your messages' `from_email` domains always match a configured
Mailgun sender domain, this setting is not needed.

See :ref:`mailgun-sender-domain` below for examples.


.. setting:: ANYMAIL_MAILGUN_API_URL

.. rubric:: MAILGUN_API_URL

The base url for calling the Mailgun API. It does not include
the sender domain. (Anymail :ref:`figures this out <mailgun-sender-domain>`
for you.)

The default is ``MAILGUN_API_URL = "https://api.mailgun.net/v3"``
(It's unlikely you would need to change this.)


.. _mailgun-sender-domain:

Email sender domain
-------------------

Mailgun's API requires identifying the sender domain.
By default, Anymail uses the domain of each messages's `from_email`
(e.g., "example.com" for "from\@example.com").

You will need to override this default if you are using
a dedicated `Mailgun sender domain`_ that is different from
a message's `from_email` domain.

For example, if you are sending from "orders\@example.com", but your
Mailgun account is configured for "*mail1*.example.com", you should provide
:setting:`MAILGUN_SENDER_DOMAIN <ANYMAIL_MAILGUN_SENDER_DOMAIN>` in your settings.py:

    .. code-block:: python
        :emphasize-lines: 4

        ANYMAIL = {
            ...
            "MAILGUN_API_KEY": "<your API key>",
            "MAILGUN_SENDER_DOMAIN": "mail1.example.com"
        }


If you need to override the sender domain for an individual message,
include `sender_domain` in Anymail's :attr:`~anymail.message.AnymailMessage.esp_extra`
for that message:

    .. code-block:: python

        message = EmailMessage(from_email="marketing@example.com", ...)
        message.esp_extra = {"sender_domain": "mail2.example.com"}


.. _Mailgun sender domain:
    https://help.mailgun.com/hc/en-us/articles/202256730-How-do-I-pick-a-domain-name-for-my-Mailgun-account-


.. _mailgun-esp-extra:

exp_extra support
-----------------

Anymail's Mailgun backend will pass all :attr:`~anymail.message.AnymailMessage.esp_extra`
values directly to Mailgun. You can use any of the (non-file) parameters listed in the
`Mailgun sending docs`_. Example:

  .. code-block:: python

      message = AnymailMessage(...)
      message.esp_extra = {
          'o:testmode': 'yes',  # use Mailgun's test mode
      }

.. _Mailgun sending docs: https://documentation.mailgun.com/api-sending.html#sending


.. _mailgun-quirks:

Limitations and quirks
----------------------

**Metadata keys and tracking webhooks**
  Because of the way Mailgun supplies custom data (user-variables) to webhooks,
  there are a few metadata keys that Anymail cannot reliably retrieve in some
  tracking events. You should avoid using "body-plain", "h", "message-headers",
  "message-id" or "tag" as :attr:`~anymail.message.AnymailMessage.metadata` keys
  if you need to access that metadata from an opened, clicked, or unsubscribed
  :ref:`tracking event <event-tracking>` handler.


.. _mailgun-templates:

Batch sending/merge and ESP templates
-------------------------------------

Mailgun does not offer :ref:`ESP stored templates <esp-stored-templates>`,
so Anymail's :attr:`~anymail.message.AnymailMessage.template_id` message
attribute is not supported with the Mailgun backend.

Mailgun *does* support :ref:`batch sending <batch-send>` with per-recipient
merge data. You can refer to Mailgun "recipient variables" in your
message subject and body, and supply the values with Anymail's
normalized :attr:`~anymail.message.AnymailMessage.merge_data`
and :attr:`~anymail.message.AnymailMessage.merge_global_data`
message attributes:

  .. code-block:: python

      message = EmailMessage(
          ...
          subject="Your order %recipient.order_no% has shipped",
          body="""Hi %recipient.name%,
                  We shipped your order %recipient.order_no%
                  on %recipient.ship_date%.""",
          to=["alice@example.com", "Bob <bob@example.com>"]
      )
      # (you'd probably also set a similar html body with %recipient.___% variables)
      message.merge_data = {
          'alice@example.com': {'name': "Alice", 'order_no': "12345"},
          'bob@example.com': {'name': "Bob", 'order_no': "54321"},
      }
      message.merge_global_data = {
          'ship_date': "May 15"  # Anymail maps globals to all recipients
      }

Mailgun does not natively support global merge data. Anymail emulates
the capability by copying any `merge_global_data` values to each
recipient's section in Mailgun's "recipient-variables" API parameter.

See the `Mailgun batch sending`_ docs for more information.

.. _Mailgun batch sending:
    https://documentation.mailgun.com/user_manual.html#batch-sending


.. _mailgun-webhooks:

Status tracking webhooks
------------------------

If you are using Anymail's normalized :ref:`status tracking <event-tracking>`, enter
the url in your `Mailgun dashboard`_ on the "Webhooks" tab. Mailgun allows you to enter
a different URL for each event type: just enter this same Anymail tracking URL
for all events you want to receive:

   :samp:`https://{random}:{random}@{yoursite.example.com}/anymail/mailgun/tracking/`

     * *random:random* is an :setting:`ANYMAIL_WEBHOOK_AUTHORIZATION` shared secret
     * *yoursite.example.com* is your Django site

If you use multiple Mailgun sending domains, you'll need to enter the webhook
URLs for each of them, using the selector on the left side of Mailgun's dashboard.

Mailgun implements a limited form of webhook signing, and Anymail will verify
these signatures (based on your :setting:`MAILGUN_API_KEY <ANYMAIL_MAILGUN_API_KEY>`
Anymail setting).

Mailgun will report these Anymail :attr:`~anymail.signals.AnymailTrackingEvent.event_type`\s:
delivered, rejected, bounced, complained, unsubscribed, opened, clicked.

The event's :attr:`~anymail.signals.AnymailTrackingEvent.esp_event` field will be
a Django :class:`~django.http.QueryDict` object of `Mailgun event fields`_.

.. _Mailgun dashboard: https://mailgun.com/app/dashboard
.. _Mailgun event fields: https://documentation.mailgun.com/user_manual.html#webhooks


.. _mailgun-inbound:

Inbound webhook
---------------

If you want to receive email from Mailgun through Anymail's normalized :ref:`inbound <inbound>`
handling, follow Mailgun's `Receiving, Storing and Fowarding Messages`_ guide to set up
an inbound route that forwards to Anymail's inbound webhook. (You can configure routes
using Mailgun's API, or simply using the "Routes" tab in your `Mailgun dashboard`_.)

The *action* for your route will be either:

   :samp:`forward("https://{random}:{random}@{yoursite.example.com}/anymail/mailgun/inbound/")`
   :samp:`forward("https://{random}:{random}@{yoursite.example.com}/anymail/mailgun/inbound_mime/")`

     * *random:random* is an :setting:`ANYMAIL_WEBHOOK_AUTHORIZATION` shared secret
     * *yoursite.example.com* is your Django site

Anymail accepts either of Mailgun's "fully-parsed" (.../inbound/) and "raw MIME" (.../inbound_mime/)
formats; the URL tells Mailgun which you want. Because Anymail handles parsing and normalizing the data,
both are equally easy to use. The raw MIME option will give the most accurate representation of *any*
received email (including complex forms like multi-message mailing list digests). The fully-parsed option
*may* use less memory while processing messages with many large attachments.

If you want to use Anymail's normalized :attr:`~anymail.inbound.AnymailInboundMessage.spam_detected` and
:attr:`~anymail.inbound.AnymailInboundMessage.spam_score` attributes, you'll need to set your Mailgun
domain's inbound spam filter to "Deliver spam, but add X-Mailgun-SFlag and X-Mailgun-SScore headers"
(in the `Mailgun dashboard`_ on the "Domains" tab).

.. _Receiving, Storing and Fowarding Messages:
   https://documentation.mailgun.com/en/latest/user_manual.html#receiving-forwarding-and-storing-messages

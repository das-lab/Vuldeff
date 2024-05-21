.. _sendgrid-backend:

SendGrid
========

Anymail integrates with the `SendGrid`_ email service, using their `Web API v3`_.

.. versionchanged:: 0.8

    Earlier Anymail releases used SendGrid's v2 API. If you are upgrading,
    please review the :ref:`porting notes <sendgrid-v3-upgrade>`.

.. important::

    **Troubleshooting:**
    If your SendGrid messages aren't being delivered as expected, be sure to look for
    "drop" events in your SendGrid `activity feed`_.

    SendGrid detects certain types of errors only *after* the send API call appears
    to succeed, and reports these errors as drop events.

.. _SendGrid: https://sendgrid.com/
.. _Web API v3: https://sendgrid.com/docs/API_Reference/Web_API_v3/Mail/index.html
.. _activity feed: https://app.sendgrid.com/email_activity?events=drops


Settings
--------


.. rubric:: EMAIL_BACKEND

To use Anymail's SendGrid backend, set:

  .. code-block:: python

      EMAIL_BACKEND = "anymail.backends.sendgrid.EmailBackend"

in your settings.py.


.. setting:: ANYMAIL_SENDGRID_API_KEY

.. rubric:: SENDGRID_API_KEY

A SendGrid API key with "Mail Send" permission.
(Manage API keys in your `SendGrid API key settings`_.)
Required.

  .. code-block:: python

      ANYMAIL = {
          ...
          "SENDGRID_API_KEY": "<your API key>",
      }

Anymail will also look for ``SENDGRID_API_KEY`` at the
root of the settings file if neither ``ANYMAIL["SENDGRID_API_KEY"]``
nor ``ANYMAIL_SENDGRID_API_KEY`` is set.

.. _SendGrid API key settings: https://app.sendgrid.com/settings/api_keys


.. setting:: ANYMAIL_SENDGRID_GENERATE_MESSAGE_ID

.. rubric:: SENDGRID_GENERATE_MESSAGE_ID

Whether Anymail should generate a Message-ID for messages sent
through SendGrid, to facilitate event tracking.

Default ``True``. You can set to ``False`` to disable this behavior.
See :ref:`Message-ID quirks <sendgrid-message-id>` below.


.. setting:: ANYMAIL_SENDGRID_MERGE_FIELD_FORMAT

.. rubric:: SENDGRID_MERGE_FIELD_FORMAT

If you use :ref:`merge data <merge-data>`, set this to a :meth:`str.format`
formatting string that indicates how merge fields are delimited
in your SendGrid templates.
For example, if your templates use the ``-field-`` hyphen delimiters
suggested in some SendGrid docs, you would set:

  .. code-block:: python

      ANYMAIL = {
          ...
          "SENDGRID_MERGE_FIELD_FORMAT": "-{}-",
      }

The placeholder `{}` will become the merge field name. If you need to include
a literal brace character, double it up. (For example, Handlebars-style
``{{field}}`` delimiters would take the format string `"{{{{{}}}}}"`.)

The default `None` requires you include the delimiters directly in your
:attr:`~anymail.message.AnymailMessage.merge_data` keys.
You can also override this setting for individual messages.
See the notes on SendGrid :ref:`templates and merge <sendgrid-templates>`
below.


.. setting:: ANYMAIL_SENDGRID_API_URL

.. rubric:: SENDGRID_API_URL

The base url for calling the SendGrid API.

The default is ``SENDGRID_API_URL = "https://api.sendgrid.com/v3/"``
(It's unlikely you would need to change this.)


.. _sendgrid-esp-extra:

esp_extra support
-----------------

To use SendGrid features not directly supported by Anymail, you can
set a message's :attr:`~anymail.message.AnymailMessage.esp_extra` to
a `dict` of parameters for SendGrid's `v3 Mail Send API`_.
Your :attr:`esp_extra` dict will be deeply merged into the
parameters Anymail has constructed for the send, with `esp_extra`
having precedence in conflicts.

Example:

    .. code-block:: python

        message.open_tracking = True
        message.esp_extra = {
            "asm": {  # SendGrid subscription management
                "group_id": 1,
                "groups_to_display": [1, 2, 3],
            },
            "tracking_settings": {
                "open_tracking": {
                    # Anymail will automatically set `"enable": True` here,
                    # based on message.open_tracking.
                    "substitution_tag": "%%OPEN_TRACKING_PIXEL%%",
                },
            },
        }


(You can also set `"esp_extra"` in Anymail's
:ref:`global send defaults <send-defaults>` to apply it to all
messages.)


.. _v3 Mail Send API:
    https://sendgrid.com/docs/API_Reference/Web_API_v3/Mail/index.html#-Request-Body-Parameters



Limitations and quirks
----------------------

.. _sendgrid-message-id:

**Message-ID**
  SendGrid does not return any sort of unique id from its send API call.
  Knowing a sent message's ID can be important for later queries about
  the message's status.

  To work around this, Anymail by default generates a new Message-ID for each
  outgoing message, provides it to SendGrid, and includes it in the
  :attr:`~anymail.message.AnymailMessage.anymail_status`
  attribute after you send the message.

  In later SendGrid API calls, you can match that Message-ID
  to SendGrid's ``smtp-id`` event field. (Anymail uses an additional
  workaround to ensure smtp-id is included in all SendGrid events,
  even those that aren't documented to include it.)

  Anymail will use the domain of the message's :attr:`from_email`
  to generate the Message-ID. (If this isn't desired, you can supply
  your own Message-ID in the message's :attr:`extra_headers`.)

  To disable all of these Message-ID workarounds, set
  :setting:`ANYMAIL_SENDGRID_GENERATE_MESSAGE_ID` to False in your settings.

**Single Reply-To**
  SendGrid's v3 API only supports a single Reply-To address (and blocks
  a workaround that was possible with the v2 API).

  If your message has multiple reply addresses, you'll get an
  :exc:`~anymail.exceptions.AnymailUnsupportedFeature` error---or
  if you've enabled :setting:`ANYMAIL_IGNORE_UNSUPPORTED_FEATURES`,
  Anymail will use only the first one.

**Invalid Addresses**
  SendGrid will accept *and send* just about anything as
  a message's :attr:`from_email`. (And email protocols are
  actually OK with that.)

  (Tested March, 2016)


.. _sendgrid-templates:

Batch sending/merge and ESP templates
-------------------------------------

SendGrid offers both :ref:`ESP stored templates <esp-stored-templates>`
and :ref:`batch sending <batch-send>` with per-recipient merge data.

You can use a SendGrid stored template by setting a message's
:attr:`~anymail.message.AnymailMessage.template_id` to the
template's unique id. Alternatively, you can refer to merge fields
directly in an EmailMessage's subject and body---the message itself
is used as an on-the-fly template.

In either case, supply the merge data values with Anymail's
normalized :attr:`~anymail.message.AnymailMessage.merge_data`
and :attr:`~anymail.message.AnymailMessage.merge_global_data`
message attributes.

  .. code-block:: python

      message = EmailMessage(
          ...
          # omit subject and body (or set to None) to use template content
          to=["alice@example.com", "Bob <bob@example.com>"]
      )
      message.template_id = "5997fcf6-2b9f-484d-acd5-7e9a99f0dc1f"  # SendGrid id
      message.merge_data = {
          'alice@example.com': {'name': "Alice", 'order_no': "12345"},
          'bob@example.com': {'name': "Bob", 'order_no': "54321"},
      }
      message.merge_global_data = {
          'ship_date': "May 15",
      }
      message.esp_extra = {
          # Tell Anymail this SendGrid template uses "-field-" to refer to merge fields.
          # (We could also just set SENDGRID_MERGE_FIELD_FORMAT in our ANYMAIL settings.)
          'merge_field_format': "-{}-"
      }

SendGrid doesn't have a pre-defined merge field syntax, so you
must tell Anymail how substitution fields are delimited in your templates.
There are three ways you can do this:

  * Set `'merge_field_format'` in the message's
    :attr:`~anymail.message.AnymailMessage.esp_extra` to a python :meth:`str.format`
    string, as shown in the example above. (This applies only to that
    particular EmailMessage.)
  * *Or* set :setting:`SENDGRID_MERGE_FIELD_FORMAT <ANYMAIL_SENDGRID_MERGE_FIELD_FORMAT>`
    in your Anymail settings. This is usually the best approach, and will apply to all messages
    sent through SendGrid. (You can still use esp_extra to override for individual messages.)
  * *Or* include the field delimiters directly in *all* your
    :attr:`~anymail.message.AnymailMessage.merge_data` and
    :attr:`~anymail.message.AnymailMessage.merge_global_data` keys.
    E.g.: ``{'-name-': "Alice", '-order_no-': "12345"}``.
    (This can be error-prone, and difficult to move to other ESPs.)

When you supply per-recipient :attr:`~anymail.message.AnymailMessage.merge_data`,
Anymail automatically changes how it communicates the "to" list to SendGrid, so that
so that each recipient sees only their own email address. (Anymail creates a separate
"personalization" for each recipient in the "to" list; any cc's or bcc's will be
duplicated for *every* to-recipient.)

SendGrid templates allow you to mix your EmailMessage's `subject` and `body`
with the template subject and body (by using `<%subject%>` and `<%body%>` in
your SendGrid template definition where you want the message-specific versions
to appear). If you don't want to supply any additional subject or body content
from your Django app, set those EmailMessage attributes to empty strings or `None`.

See the `SendGrid's template overview`_ and `transactional template docs`_
for more information.

.. _SendGrid's template overview:
    https://sendgrid.com/docs/User_Guide/Transactional_Templates/index.html
.. _transactional template docs:
    https://sendgrid.com/docs/API_Reference/Web_API_v3/Transactional_Templates/smtpapi.html


.. _sendgrid-webhooks:

Status tracking webhooks
------------------------

If you are using Anymail's normalized :ref:`status tracking <event-tracking>`, enter
the url in your `SendGrid mail settings`_, under "Event Notification":

   :samp:`https://{random}:{random}@{yoursite.example.com}/anymail/sendgrid/tracking/`

     * *random:random* is an :setting:`ANYMAIL_WEBHOOK_AUTHORIZATION` shared secret
     * *yoursite.example.com* is your Django site

Be sure to check the boxes in the SendGrid settings for the event types you want to receive.

SendGrid will report these Anymail :attr:`~anymail.signals.AnymailTrackingEvent.event_type`\s:
queued, rejected, bounced, deferred, delivered, opened, clicked, complained, unsubscribed,
subscribed.

The event's :attr:`~anymail.signals.AnymailTrackingEvent.esp_event` field will be
a `dict` of `Sendgrid event`_ fields, for a single event. (Although SendGrid calls
webhooks with batches of events, Anymail will invoke your signal receiver separately
for each event in the batch.)

.. _SendGrid mail settings: https://app.sendgrid.com/settings/mail_settings
.. _Sendgrid event: https://sendgrid.com/docs/API_Reference/Webhooks/event.html


.. _sendgrid-inbound:

Inbound webhook
---------------

If you want to receive email from SendGrid through Anymail's normalized :ref:`inbound <inbound>`
handling, follow SendGrid's `Inbound Parse Webhook`_ guide to set up
Anymail's inbound webhook.

The Destination URL setting will be:

   :samp:`https://{random}:{random}@{yoursite.example.com}/anymail/sendgrid/inbound/`

     * *random:random* is an :setting:`ANYMAIL_WEBHOOK_AUTHORIZATION` shared secret
     * *yoursite.example.com* is your Django site

Be sure the URL has a trailing slash. (SendGrid's inbound processing won't follow Django's
:setting:`APPEND_SLASH` redirect.)

If you want to use Anymail's normalized :attr:`~anymail.inbound.AnymailInboundMessage.spam_detected` and
:attr:`~anymail.inbound.AnymailInboundMessage.spam_score` attributes, be sure to enable the "Check
incoming emails for spam" checkbox.

You have a choice for SendGrid's "POST the raw, full MIME message" checkbox. Anymail will handle
either option (and you can change it at any time). Enabling raw MIME will give the most accurate
representation of *any* received email (including complex forms like multi-message mailing list
digests). But disabling it *may* use less memory while processing messages with many large attachments.

.. _Inbound Parse Webhook:
   https://sendgrid.com/docs/Classroom/Basics/Inbound_Parse_Webhook/setting_up_the_inbound_parse_webhook.html


.. _sendgrid-v3-upgrade:

Upgrading to SendGrid's v3 API
------------------------------

Anymail v0.8 switched to SendGrid's preferred v3 send API.
(Earlier Anymail releases used their v2 API.)

For many Anymail projects, this change will be entirely transparent.
(Anymail's whole reason for existence is abstracting ESP APIs,
so that your own code doesn't need to worry about the details.)

There are three cases where SendGrid has changed features
that would require updates to your code:

1. If you are using SendGrid's username/password auth (your settings
   include :setting:`SENDGRID_USERNAME <ANYMAIL_SENDGRID_USERNAME>`
   and :setting:`SENDGRID_PASSWORD <ANYMAIL_SENDGRID_PASSWORD>`),
   you must switch to an API key.
   See :setting:`SENDGRID_API_KEY <ANYMAIL_SENDGRID_API_KEY>`.

   (If you are already using a SendGrid API key with v2, it should
   work just fine with v3.)

2. If you are using Anymail's
   :attr:`~anymail.message.AnymailMessage.esp_extra` attribute
   to supply API-specific parameters, the format has changed.

   Search your code for "esp_extra" (e.g., `git grep esp_extra`)
   to determine whether this affects you. (Anymail's
   `"merge_field_format"` is unchanged, so if that's the only
   thing you have in esp_extra, no changes are needed.)

   The new API format is considerably simpler and more logical.
   See :ref:`sendgrid-esp-extra` below for examples of the
   new format and a link to relevant SendGrid docs.

   Anymail will raise an error if it detects an attempt to use
   the v2-only `"x-smtpapi"` settings in esp_extra when sending.

3. If you send messages with multiple Reply-To addresses, SendGrid
   no longer supports this. (Multiple reply emails in a single
   message are not common.)

   Anymail will raise an error if you attempt to send a message with
   multiple Reply-To emails. (You can suppress the error with
   :setting:`ANYMAIL_IGNORE_UNSUPPORTED_FEATURES`, which will
   ignore all but the first reply address.)


As an alternative, Anymail (for the time being) still includes
a copy of the SendGrid v2 backend. See :ref:`sendgrid-v2-backend`
below if you'd prefer to stay on the older SendGrid API.


.. _sendgrid-v2-backend:

Legacy v2 API support
---------------------

.. versionchanged:: 0.8

Anymail v0.8 switched to SendGrid's v3 Web API in its primary SendGrid
email backend. SendGrid `encourages`_ all users to migrate to their v3 API.

For Anymail users who still need it, a legacy backend that calls SendGrid's
earlier `Web API v2 Mail Send`_ remains available. Be aware that v2 support
is considered deprecated and may be removed in a future Anymail release.

.. _encourages:
    https://sendgrid.com/docs/Classroom/Send/v3_Mail_Send/how_to_migrate_from_v2_to_v3_mail_send.html
.. _Web API v2 Mail Send:
    https://sendgrid.com/docs/API_Reference/Web_API/mail.html


To use Anymail's SendGrid v2 backend, edit your settings.py:

  .. code-block:: python

      EMAIL_BACKEND = "anymail.backends.sendgrid_v2.EmailBackend"
      ANYMAIL = {
          "SENDGRID_API_KEY": "<your API key>",
      }

The same :setting:`SENDGRID_API_KEY <ANYMAIL_SENDGRID_API_KEY>` will work
with either Anymail's v2 or v3 SendGrid backend.

Nearly all of the documentation above for Anymail's v3 SendGrid backend
also applies to the v2 backend, with the following changes:

.. setting:: ANYMAIL_SENDGRID_USERNAME
.. setting:: ANYMAIL_SENDGRID_PASSWORD

.. rubric:: Username/password auth (SendGrid v2 only)

SendGrid v2 allows a username/password instead of an API key
(though SendGrid encourages API keys for all new installations).
If you must use username/password auth, set:

  .. code-block:: python

      EMAIL_BACKEND = "anymail.backends.sendgrid_v2.EmailBackend"
      ANYMAIL = {
          "SENDGRID_USERNAME": "<sendgrid credential with Mail permission>",
          "SENDGRID_PASSWORD": "<password for that credential>",
          # And leave out "SENDGRID_API_KEY"
      }

This is **not** the username/password that you use to log into SendGrid's
dashboard. Create credentials specifically for sending mail in the
`SendGrid credentials settings`_.

Either username/password or :setting:`SENDGRID_API_KEY <ANYMAIL_SENDGRID_API_KEY>`
are required (but not both).

Anymail will also look for ``SENDGRID_USERNAME`` and ``SENDGRID_PASSWORD`` at the
root of the settings file if neither ``ANYMAIL["SENDGRID_USERNAME"]``
nor ``ANYMAIL_SENDGRID_USERNAME`` is set.

.. _SendGrid credentials settings: https://app.sendgrid.com/settings/credentials


.. rubric:: Duplicate attachment filenames (SendGrid v2 limitation)

Anymail is not capable of communicating multiple attachments with
the same filename to the SendGrid v2 API. (This also applies to multiple
attachments with *no* filename, though not to inline images.)

If you are sending multiple attachments on a single message,
make sure each one has a unique, non-empty filename.


.. rubric:: Message bodies with ESP templates (SendGrid v2 quirk)

Anymail's SendGrid v2 backend will convert empty text and HTML bodies to single spaces whenever
:attr:`~anymail.message.AnymailMessage.template_id` is set, to ensure the
plaintext and HTML from your template are present in your outgoing email.
This works around a `limitation in SendGrid's template rendering`_.

.. _limitation in SendGrid's template rendering:
    https://sendgrid.com/docs/API_Reference/Web_API_v3/Transactional_Templates/smtpapi.html#-Text-or-HTML-Templates


.. rubric:: Multiple Reply-To addresses (SendGrid v2 only)

Unlike SendGrid's v3 API, Anymail is able to support multiple
Reply-To addresses with their v2 API.


.. rubric:: esp_extra with SendGrid v2

Anymail's :attr:`~anymail.message.AnymailMessage.esp_extra` attribute
is merged directly with the API parameters, so the format varies
between SendGrid's v2 and v3 APIs. With the v2 API, most interesting
settings appear beneath `'x-smtpapi'`. Example:

    .. code-block:: python

        message.esp_extra = {
            'x-smtpapi': {  # for SendGrid v2 API
                "asm_group": 1,  # Assign SendGrid unsubscribe group for this message
                "asm_groups_to_display": [1, 2, 3],
                "filters": {
                    "subscriptiontrack": {  # Insert SendGrid subscription management links
                        "settings": {
                            "text/html": "If you would like to unsubscribe <% click here %>.",
                            "text/plain": "If you would like to unsubscribe click here: <% %>.",
                            "enable": 1
                        }
                    }
                }
            }
        }

The value of :attr:`esp_extra` should be a `dict` of parameters for SendGrid's
`v2 mail.send API`_. Any keys in the dict will override Anymail's normal values
for that parameter, except that `'x-smtpapi'` will be merged.

.. _v2 mail.send API:
    https://sendgrid.com/docs/API_Reference/Web_API/mail.html#-send

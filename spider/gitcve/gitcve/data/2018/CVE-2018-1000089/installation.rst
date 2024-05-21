Installation and configuration
==============================

.. _installation:

Installing Anymail
------------------

To use Anymail in your Django project:

1. Install the django-anymail app. It's easiest to install from PyPI using pip:

    .. code-block:: console

        $ pip install django-anymail[sendgrid,sparkpost]

   The `[sendgrid,sparkpost]` part of that command tells pip you also
   want to install additional packages required for those ESPs.
   You can give one or more comma-separated, lowercase ESP names.
   (Most ESPs don't have additional requirements, so you can often
   just skip this. Or change your mind later. Anymail will let you know
   if there are any missing dependencies when you try to use it.)

2. Edit your Django project's :file:`settings.py`, and add :mod:`anymail`
   to your :setting:`INSTALLED_APPS` (anywhere in the list):

    .. code-block:: python

        INSTALLED_APPS = [
            # ...
            "anymail",
            # ...
        ]

3. Also in :file:`settings.py`, add an :setting:`ANYMAIL` settings dict,
   substituting the appropriate settings for your ESP. E.g.:

    .. code-block:: python

        ANYMAIL = {
            "MAILGUN_API_KEY": "<your Mailgun key>",
        }

   The exact settings vary by ESP.
   See the :ref:`supported ESPs <supported-esps>` section for specifics.

Then continue with either or both of the next two sections, depending
on which Anymail features you want to use.


.. _backend-configuration:

Configuring Django's email backend
----------------------------------

To use Anymail for *sending* email from Django, make additional changes
in your project's :file:`settings.py`. (Skip this section if you are only
planning to *receive* email.)

1. Change your existing Django :setting:`EMAIL_BACKEND` to the Anymail backend
   for your ESP. For example, to send using Mailgun by default:

    .. code-block:: python

        EMAIL_BACKEND = "anymail.backends.mailgun.EmailBackend"

   (:setting:`EMAIL_BACKEND` sets Django's default for sending emails; you can also
   use :ref:`multiple Anymail backends <multiple-backends>` to send particular
   messages through different ESPs.)

2. If you don't already have a :setting:`DEFAULT_FROM_EMAIL` in your settings,
   this is a good time to add one. (Django's default is "webmaster\@localhost",
   which some ESPs will reject.)

With the settings above, you are ready to send outgoing email through your ESP.
If you also want to enable status tracking or inbound handling, continue with the
settings below. Otherwise, skip ahead to :ref:`sending-email`.


.. _webhooks-configuration:

Configuring tracking and inbound webhooks
-----------------------------------------

Anymail can optionally connect to your ESP's event webhooks to notify your app of:

* status tracking events for sent email, like bounced or rejected messages,
  successful delivery, message opens and clicks, etc.
* inbound message events, if you are set up to receive email through your ESP

Skip this section if you won't be using Anymail's webhooks.

.. warning::

    Webhooks are ordinary urls, and are wide open to the internet.
    You must use care to **avoid creating security vulnerabilities**
    that could expose your users' emails and other private information,
    or subject your app to malicious input data.

    At a minimum, your site should **use https** and you should
    configure **webhook authorization** as described below.

    See :ref:`securing-webhooks` for additional information.


If you want to use Anymail's inbound or tracking webhooks:

1. In your :file:`settings.py`, add
   :setting:`WEBHOOK_AUTHORIZATION <ANYMAIL_WEBHOOK_AUTHORIZATION>`
   to the ``ANYMAIL`` block:

   .. code-block:: python

      ANYMAIL = {
          ...
          'WEBHOOK_AUTHORIZATION': '<a random string>:<another random string>',
      }

   This setting should be a string with two sequences of random characters,
   separated by a colon. It is used as a shared secret, known only to your ESP
   and your Django app, to ensure nobody else can call your webhooks.

   We suggest using 16 characters (or more) for each half of the
   secret. Always generate a new, random secret just for this purpose.
   (*Don't* use your Django secret key or ESP's API key.)

   An easy way to generate a random secret is to run this command in
   a shell:

   .. code-block:: console

      $ python -c "from django.utils import crypto; print(':'.join(crypto.get_random_string(16) for _ in range(2)))"

   (This setting is actually an HTTP basic auth string. You can also set it
   to a list of auth strings, to simplify credential rotation or use different auth
   with different ESPs. See :setting:`ANYMAIL_WEBHOOK_AUTHORIZATION` in the
   :ref:`securing-webhooks` docs for more details.)


2. In your project's :file:`urls.py`, add routing for the Anymail webhook urls:

   .. code-block:: python

      from django.conf.urls import include, url

      urlpatterns = [
          ...
          url(r'^anymail/', include('anymail.urls')),
      ]

   (You can change the "anymail" prefix in the first parameter to
   :func:`~django.conf.urls.url` if you'd like the webhooks to be served
   at some other URL. Just match whatever you use in the webhook URL you give
   your ESP in the next step.)


3. Enter the webhook URL(s) into your ESP's dashboard or control panel.
   In most cases, the URL will be:

   :samp:`https://{random}:{random}@{yoursite.example.com}/anymail/{esp}/{type}/`

     * "https" (rather than http) is *strongly recommended*
     * *random:random* is the WEBHOOK_AUTHORIZATION string you created in step 1
     * *yoursite.example.com* is your Django site
     * "anymail" is the url prefix (from step 2)
     * *esp* is the lowercase name of your ESP (e.g., "sendgrid" or "mailgun")
     * *type* is either "tracking" for Anymail's sent-mail event tracking webhooks,
       or "inbound" for receiving email

   Some ESPs support different webhooks for different tracking events. You can
   usually enter the same Anymail *tracking* webhook URL for all of them (or all that you
   want to receive)---but be sure to use the separate *inbound* URL for inbound webhooks.
   And always check the specific details for your ESP under :ref:`supported-esps`.

   Also, some ESPs try to validate the webhook URL immediately when you enter it.
   If so, you'll need to deploy your Django project to your live server before you
   can complete this step.

Some WSGI servers may need additional settings to pass HTTP authorization headers
through to Django. For example, Apache with `mod_wsgi`_ requires
`WSGIPassAuthorization On`, else Anymail will complain about "missing or invalid
basic auth" when your webhook is called.

See :ref:`event-tracking` for information on creating signal handlers and the
status tracking events you can receive. See :ref:`inbound` for information on
receiving inbound message events.

.. _mod_wsgi: http://modwsgi.readthedocs.io/en/latest/configuration-directives/WSGIPassAuthorization.html


.. setting:: ANYMAIL

Anymail settings reference
--------------------------

You can add Anymail settings to your project's :file:`settings.py` either as
a single ``ANYMAIL`` dict, or by breaking out individual settings prefixed with
``ANYMAIL_``. So this settings dict:

    .. code-block:: python

        ANYMAIL = {
            "MAILGUN_API_KEY": "12345",
            "SEND_DEFAULTS": {
                "tags": ["myapp"]
            },
        }

...is equivalent to these individual settings:

    .. code-block:: python

        ANYMAIL_MAILGUN_API_KEY = "12345"
        ANYMAIL_SEND_DEFAULTS = {"tags": ["myapp"]}

In addition, for some ESP settings like API keys, Anymail will look for a setting
without the ``ANYMAIL_`` prefix if it can't find the Anymail one. (This can be helpful
if you are using other Django apps that work with the same ESP.)

    .. code-block:: python

        MAILGUN_API_KEY = "12345"  # used only if neither ANYMAIL["MAILGUN_API_KEY"]
                                   # nor ANYMAIL_MAILGUN_API_KEY have been set


Finally, for complex use cases, you can override most settings on a per-instance
basis by providing keyword args where the instance is initialized (e.g., in a
:func:`~django.core.mail.get_connection` call to create an email backend instance,
or in `View.as_view()` call to set up webhooks in a custom urls.py). To get the kwargs
parameter for a setting, drop "ANYMAIL" and the ESP name, and lowercase the rest:
e.g., you can override ANYMAIL_MAILGUN_API_KEY by passing `api_key="abc"` to
:func:`~django.core.mail.get_connection`. See :ref:`multiple-backends` for an example.

There are specific Anymail settings for each ESP (like API keys and urls).
See the :ref:`supported ESPs <supported-esps>` section for details.
Here are the other settings Anymail supports:


.. setting:: ANYMAIL_IGNORE_RECIPIENT_STATUS

.. rubric:: IGNORE_RECIPIENT_STATUS

Set to `True` to disable :exc:`AnymailRecipientsRefused` exceptions
on invalid or rejected recipients. (Default `False`.)
See :ref:`recipients-refused`.

  .. code-block:: python

      ANYMAIL = {
          ...
          "IGNORE_RECIPIENT_STATUS": True,
      }


.. rubric:: SEND_DEFAULTS and *ESP*\ _SEND_DEFAULTS`

A `dict` of default options to apply to all messages sent through Anymail.
See :ref:`send-defaults`.


.. rubric:: IGNORE_UNSUPPORTED_FEATURES

Whether Anymail should raise :exc:`~anymail.exceptions.AnymailUnsupportedFeature`
errors for email with features that can't be accurately communicated to the ESP.
Set to `True` to ignore these problems and send the email anyway. See
:ref:`unsupported-features`. (Default `False`.)


.. rubric:: WEBHOOK_AUTHORIZATION

A `'random:random'` shared secret string. Anymail will reject incoming webhook calls
from your ESP that don't include this authorization. You can also give a list of
shared secret strings, and Anymail will allow ESP webhook calls that match any of them
(to facilitate credential rotation). See :ref:`securing-webhooks`.

Default is unset, which leaves your webhooks insecure. Anymail
will warn if you try to use webhooks with setting up authorization.

This is actually implemented using HTTP basic authorization, and the string is
technically a "username:password" format. But you should *not* use any real
username or password for this shared secret.


.. setting:: ANYMAIL_REQUESTS_TIMEOUT

.. rubric:: REQUESTS_TIMEOUT

.. versionadded:: 1.3

For Requests-based Anymail backends, the timeout value used for all API calls to your ESP.
The default is 30 seconds. You can set to a single float, a 2-tuple of floats for
separate connection and read timeouts, or `None` to disable timeouts (not recommended).
See :ref:`requests:timeouts` in the Requests docs for more information.

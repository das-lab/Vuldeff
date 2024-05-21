.. _securing-webhooks:

Securing webhooks
=================

If not used carefully, webhooks can create security vulnerabilities
in your Django application.

At minimum, you should **use SSL** and a **shared authorization secret**
for your Anymail webhooks. (Really, for *any* webhooks.)


Use SSL
-------

Your Django site must use SSL, and the webhook URLs you
give your ESP should start with "https" (not http).

Without https, the data your ESP sends your webhooks is exposed in transit.
This can include your customers' email addresses, the contents of messages
you receive through your ESP, the shared secret used to authorize calls
to your webhooks (described in the next section), and other data you'd
probably like to keep private.

Configuring SSL is beyond the scope of Anymail, but there are many good
tutorials on the web.

If you aren't able to use https on your Django site, then you should
not set up your ESP's webhooks.


.. setting:: ANYMAIL_WEBHOOK_AUTHORIZATION

Use a shared authorization secret
---------------------------------

A webhook is an ordinary URL---anyone can post anything to it.
To avoid receiving random (or malicious) data in your webhook,
you should use a shared random secret that your ESP can present
with webhook data, to prove the post is coming from your ESP.

Most ESPs recommend using HTTP basic authorization as this shared
secret. Anymail includes support for this, via the
:setting:`!ANYMAIL_WEBHOOK_AUTHORIZATION` setting.
Basic usage is covered in the
:ref:`webhooks configuration <webhooks-configuration>` docs.

If something posts to your webhooks without the required shared
secret as basic auth in the HTTP_AUTHORIZATION header, Anymail will
raise an :exc:`AnymailWebhookValidationFailure` error, which is
a subclass of Django's :exc:`~django.core.exceptions.SuspiciousOperation`.
This will result in an HTTP 400 response, without further processing
the data or calling your signal receiver function.

In addition to a single "random:random" string, you can give a list
of authorization strings. Anymail will permit webhook calls that match
any of the authorization strings:

   .. code-block:: python

      ANYMAIL = {
          ...
          'WEBHOOK_AUTHORIZATION': [
              'abcdefghijklmnop:qrstuvwxyz0123456789',
              'ZYXWVUTSRQPONMLK:JIHGFEDCBA9876543210',
          ],
      }

This facilitates credential rotation: first, append a new authorization
string to the list, and deploy your Django site. Then, update the webhook
URLs at your ESP to use the new authorization. Finally, remove the old
(now unused) authorization string from the list and re-deploy.

.. warning::

    If your webhook URLs don't use https, this shared authorization
    secret won't stay secret, defeating its purpose.


Signed webhooks
---------------

Some ESPs implement webhook signing, which is another method of verifying
the webhook data came from your ESP. Anymail will verify these signatures
for ESPs that support them. See the docs for your
:ref:`specific ESP <supported-esps>` for more details and configuration
that may be required.

Even with signed webhooks, it doesn't hurt to also use a shared secret.


Additional steps
----------------

Webhooks aren't unique to Anymail or to ESPs. They're used for many
different types of inter-site communication, and you can find additional
recommendations for improving webhook security on the web.

For example, you might consider:

* Tracking :attr:`~anymail.signals.AnymailTrackingEvent.event_id`,
  to avoid accidental double-processing of the same events (or replay attacks)
* Checking the webhook's :attr:`~anymail.signals.AnymailTrackingEvent.timestamp`
  is reasonably close the current time
* Configuring your firewall to reject webhook calls that come from
  somewhere other than your ESP's documented IP addresses (if your ESP
  provides this information)

But you should start with using SSL and a random shared secret via HTTP auth.

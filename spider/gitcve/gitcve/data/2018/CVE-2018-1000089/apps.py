from django.apps import AppConfig


class AnymailBaseConfig(AppConfig):
    name = 'anymail'
    verbose_name = "Anymail"

    def ready(self):
        pass

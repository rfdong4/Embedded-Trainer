"""Entry point for python -m embedded_trainer."""

from embedded_trainer.app import EmbeddedTrainerApp


def main():
    app = EmbeddedTrainerApp()
    app.run()


if __name__ == "__main__":
    main()

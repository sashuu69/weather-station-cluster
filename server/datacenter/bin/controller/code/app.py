import psycopg2
from configparser import ConfigParser  # Library for importing .ini file
parameters_file = "parameters.ini"


class ControllerGlobal:
    def __init__(self):
        config = ConfigParser()
        config.read(parameters_file)
        self.deployment_model = config["deployment"]["model"]

        self.postgres_url = config[
            self.deployment_model]["postgres_url"]
        self.postgres_database_name = config[
            self.deployment_model]["postgres_database_name"]
        self.postgres_username = config[
            self.deployment_model]["postgres_username"]
        self.postgres_password = config[
            self.deployment_model]["postgres_password"]


def main():
    global_config = ControllerGlobal()


if __name__ == "__main__":
    main()

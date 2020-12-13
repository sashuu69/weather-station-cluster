from configparser import ConfigParser  # Library for importing .ini file
import sys

location = "parameters.ini"


def main():
    if len(sys.argv) == 1:
        print("Production type not passed...")
        exit()

    model = sys.argv[1]

    if model == "development" or model == "local":
        test = ConfigParser()
        test.read(location)
        test.set('deployment', 'model', model)

        with open(location, 'w') as configfile:
            test.write(configfile)
    else:
        print("Invalid option passed...")
        exit()


if __name__ == "__main__":
    main()

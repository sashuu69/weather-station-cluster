import pymongo

mongodb_url = "mongodb://datacenter.gisalpha.sashwat.in:27017"
myclient = pymongo.MongoClient(mongodb_url)
mydb = myclient["home-weather-station"]
mydb.auth("docker", "docker")


def main():
    print(myclient.list_database_names())


if __name__ == "__main__":
    main()

import csv


with open("people-100.csv", "r") as file:
    reader = csv.reader(file)

    for row in reader:
        print(type(row[0]))


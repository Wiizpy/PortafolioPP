class Vehicle:
    def __init__(self, plate):
        self.__plate = plate.strip().upper()

    def get_plate(self):
        return self.__plate

    def get_type(self):
        return "Generic"

    def __str__(self):
        return f"{self.get_type()} - {self.__plate}"


class Car(Vehicle):
    def get_type(self):
        return "Car"


class Motorcycle(Vehicle):
    def get_type(self):
        return "Motorcycle"
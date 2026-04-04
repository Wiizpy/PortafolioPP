class ParkingSpot:
    def __init__(self, spot_id, allowed_type):
        self.__spot_id = spot_id
        self.__allowed_type = allowed_type.capitalize()
        self.__occupied = False
        self.__current_vehicle = None

    def get_spot_id(self):
        return self.__spot_id

    def get_allowed_type(self):
        return self.__allowed_type

    def is_occupied(self):
        return self.__occupied

    def get_current_vehicle(self):
        return self.__current_vehicle

    def is_available_for(self, vehicle):
        return (not self.__occupied) and (
            self.__allowed_type == "Any" or
            self.__allowed_type == vehicle.get_type()
        )

    def park(self, vehicle):
        if self.__occupied:
            raise ValueError("El lugar ya está ocupado")
        self.__occupied = True
        self.__current_vehicle = vehicle

    def release(self):
        if not self.__occupied:
            raise ValueError("El lugar ya está libre")
        self.__occupied = False
        self.__current_vehicle = None

    def __str__(self):
        estado = "Ocupado" if self.__occupied else "Libre"
        vehiculo = ""
        if self.__current_vehicle is not None:
            vehiculo = f" | Vehículo: {self.__current_vehicle.get_plate()}"
        return f"{self.__spot_id} | Tipo permitido: {self.__allowed_type} | Estado: {estado}{vehiculo}"
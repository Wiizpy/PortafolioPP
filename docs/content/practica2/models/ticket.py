class Ticket:
    def __init__(self, ticket_id, vehicle, spot):
        self.__ticket_id = ticket_id
        self.__vehicle = vehicle
        self.__spot = spot
        self.__status = "ACTIVE"

    def get_ticket_id(self):
        return self.__ticket_id

    def get_vehicle(self):
        return self.__vehicle

    def get_spot(self):
        return self.__spot

    def get_status(self):
        return self.__status

    def close(self):
        if self.__status == "CLOSED":
            raise ValueError("El ticket ya fue cerrado")
        self.__status = "CLOSED"

    def __str__(self):
        return (
            f"Ticket #{self.__ticket_id} | "
            f"Vehículo: {self.__vehicle.get_plate()} | "
            f"Tipo: {self.__vehicle.get_type()} | "
            f"Spot: {self.__spot.get_spot_id()} | "
            f"Estado: {self.__status}"
        )
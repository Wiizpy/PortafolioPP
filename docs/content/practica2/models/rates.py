class RatePolicy:
    def calculate(self, hours, vehicle):
        raise NotImplementedError("Debes implementar el método calculate")


class HourlyRatePolicy(RatePolicy):
    def calculate(self, hours, vehicle):
        if hours <= 0:
            raise ValueError("Las horas deben ser mayores a 0")

        if vehicle.get_type() == "Car":
            return hours * 20
        elif vehicle.get_type() == "Motorcycle":
            return hours * 10
        else:
            return hours * 15


class FlatRatePolicy(RatePolicy):
    def calculate(self, hours, vehicle):
        if hours <= 0:
            raise ValueError("Las horas deben ser mayores a 0")
        return 50
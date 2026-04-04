from models.vehicle import Car, Motorcycle
from models.spot import ParkingSpot
from models.rates import HourlyRatePolicy, FlatRatePolicy
from models.parking_lot import ParkingLot


def mostrar_menu():
    print("\n===== SIMULADOR DE ESTACIONAMIENTO =====")
    print("1. Registrar entrada")
    print("2. Registrar salida")
    print("3. Ver ocupación")
    print("4. Ver tickets activos")
    print("5. Ver lugares")
    print("6. Cambiar política de cobro")
    print("7. Ver política actual")
    print("8. Salir")


def crear_estacionamiento():
    spots = [
        ParkingSpot("A1", "Car"),
        ParkingSpot("A2", "Car"),
        ParkingSpot("A3", "Car"),
        ParkingSpot("M1", "Motorcycle"),
        ParkingSpot("M2", "Motorcycle"),
        ParkingSpot("M3", "Motorcycle"),
        ParkingSpot("X1", "Any"),
        ParkingSpot("X2", "Any"),
    ]

    policy = HourlyRatePolicy()
    return ParkingLot(spots, policy)


def crear_vehiculo(plate, tipo):
    tipo = tipo.strip().capitalize()

    if tipo == "Car":
        return Car(plate)
    elif tipo == "Motorcycle":
        return Motorcycle(plate)
    else:
        raise ValueError("Tipo de vehículo inválido. Usa Car o Motorcycle")


def registrar_entrada(parking):
    plate = input("Ingresa las placas: ").strip()
    tipo = input("Ingresa el tipo de vehículo (Car/Motorcycle): ").strip()

    try:
        vehicle = crear_vehiculo(plate, tipo)
        ticket = parking.register_entry(vehicle)
        print(
            f"Entrada registrada correctamente -> "
            f"Ticket #{ticket.get_ticket_id()}, "
            f"spot={ticket.get_spot().get_spot_id()}"
        )
    except ValueError as e:
        print("Error:", e)


def registrar_salida(parking):
    try:
        ticket_id = int(input("Ingresa el ID del ticket: "))
        hours = float(input("Ingresa las horas estacionado: "))

        cost, freed_spot, vehicle_type = parking.register_exit(ticket_id, hours)
        print(
            f"Salida registrada -> ticket={ticket_id} "
            f"tipo={vehicle_type} tiempo={hours}h "
            f"costo=${cost} spot liberado={freed_spot}"
        )
    except ValueError as e:
        print("Error:", e)


def ver_ocupacion(parking):
    free, occupied = parking.get_occupancy()
    print(f"Ocupación: libres={free} ocupados={occupied}")


def ver_tickets_activos(parking):
    tickets = parking.get_active_tickets()

    if not tickets:
        print("No hay tickets activos")
        return

    print("\nTickets activos:")
    for ticket in tickets:
        print(ticket)


def ver_lugares(parking):
    print("\nEstado de lugares:")
    for spot in parking.show_spots():
        print(spot)


def cambiar_politica(parking):
    print("\nPolíticas disponibles:")
    print("1. HourlyRatePolicy")
    print("2. FlatRatePolicy")

    opcion = input("Selecciona una política: ").strip()

    if opcion == "1":
        parking.set_rate_policy(HourlyRatePolicy())
        print("Política cambiada a HourlyRatePolicy")
    elif opcion == "2":
        parking.set_rate_policy(FlatRatePolicy())
        print("Política cambiada a FlatRatePolicy")
    else:
        print("Opción inválida")


def ver_politica_actual(parking):
    print(f"Política actual: {parking.get_rate_policy_name()}")


def main():
    parking = crear_estacionamiento()

    while True:
        mostrar_menu()
        opcion = input("Selecciona una opción: ").strip()

        if opcion == "1":
            registrar_entrada(parking)
        elif opcion == "2":
            registrar_salida(parking)
        elif opcion == "3":
            ver_ocupacion(parking)
        elif opcion == "4":
            ver_tickets_activos(parking)
        elif opcion == "5":
            ver_lugares(parking)
        elif opcion == "6":
            cambiar_politica(parking)
        elif opcion == "7":
            ver_politica_actual(parking)
        elif opcion == "8":
            print("Saliendo del sistema...")
            break
        else:
            print("Opción inválida")


if __name__ == "__main__":
    main()
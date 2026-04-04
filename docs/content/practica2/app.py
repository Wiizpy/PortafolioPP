from flask import Flask, render_template, request, redirect, url_for
from models.vehicle import Car, Motorcycle
from models.spot import ParkingSpot
from models.rates import HourlyRatePolicy
from models.parking_lot import ParkingLot

app = Flask(__name__)

# Crear una sola instancia del estacionamiento
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

parking = crear_estacionamiento()


@app.route("/")
def dashboard():
    free, occupied = parking.get_occupancy()
    tickets = parking.get_active_tickets()
    spots = parking.show_spots()

    return render_template(
        "dashboard.html",
        free=free,
        occupied=occupied,
        tickets=tickets,
        spots=spots
    )


@app.route("/entry", methods=["GET", "POST"])
def entry():
    message = ""
    error = ""

    if request.method == "POST":
        plate = request.form.get("plate", "").strip()
        vehicle_type = request.form.get("vehicle_type", "").strip()

        try:
            if vehicle_type == "Car":
                vehicle = Car(plate)
            elif vehicle_type == "Motorcycle":
                vehicle = Motorcycle(plate)
            else:
                raise ValueError("Tipo de vehículo inválido")

            ticket = parking.register_entry(vehicle)
            message = f"Entrada registrada correctamente. Ticket #{ticket.get_ticket_id()} asignado en {ticket.get_spot().get_spot_id()}"
        except ValueError as e:
            error = str(e)

    return render_template("entry.html", message=message, error=error)


@app.route("/exit", methods=["GET", "POST"])
def exit_vehicle():
    message = ""
    error = ""

    if request.method == "POST":
        ticket_id = request.form.get("ticket_id", "").strip()
        hours = request.form.get("hours", "").strip()

        try:
            ticket_id = int(ticket_id)
            hours = float(hours)

            cost, freed_spot, vehicle_type = parking.register_exit(ticket_id, hours)
            message = f"Salida registrada. Ticket {ticket_id}, tipo {vehicle_type}, costo ${cost}, lugar liberado {freed_spot}"
        except ValueError as e:
            error = str(e)

    return render_template("exit.html", message=message, error=error)


if __name__ == "__main__":
    app.run(debug=True)
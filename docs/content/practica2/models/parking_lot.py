from models.ticket import Ticket


class ParkingLot:
    def __init__(self, spots, rate_policy):
        self.__spots = spots
        self.__active_tickets = {}
        self.__total_revenue = 0
        self.__next_ticket_id = 1
        self.__rate_policy = rate_policy

    def register_entry(self, vehicle):
        for spot in self.__spots:
            if spot.is_available_for(vehicle):
                spot.park(vehicle)

                ticket = Ticket(self.__next_ticket_id, vehicle, spot)
                self.__active_tickets[self.__next_ticket_id] = ticket
                self.__next_ticket_id += 1

                return ticket

        raise ValueError("No hay lugares disponibles compatibles para este vehículo")

    def register_exit(self, ticket_id, hours):
        if ticket_id not in self.__active_tickets:
            raise ValueError("El ticket no existe o no está activo")

        ticket = self.__active_tickets[ticket_id]
        vehicle = ticket.get_vehicle()
        spot = ticket.get_spot()

        cost = self.__rate_policy.calculate(hours, vehicle)

        ticket.close()
        spot.release()
        self.__total_revenue += cost

        del self.__active_tickets[ticket_id]

        return cost, spot.get_spot_id(), vehicle.get_type()

    def get_occupancy(self):
        occupied = sum(1 for spot in self.__spots if spot.is_occupied())
        free = len(self.__spots) - occupied
        return free, occupied

    def get_active_tickets(self):
        return list(self.__active_tickets.values())

    def get_total_revenue(self):
        return self.__total_revenue

    def show_spots(self):
        return self.__spots

    def set_rate_policy(self, new_policy):
        self.__rate_policy = new_policy

    def get_rate_policy_name(self):
        return self.__rate_policy.__class__.__name__
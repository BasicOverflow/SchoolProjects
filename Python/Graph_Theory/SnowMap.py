
# TODO: Define a class SnowMap that inherits from one of the data structure implemented earlier.
# Only implement one method here: plow_from

from Graph import AdjacencySetGraph



class SnowMap(AdjacencySetGraph):

    def plow_from(self, city):
        tree, D = self.dijkstra(city)

        for key in D.keys():
            if D[key] == None:
                D[key] = 0

        return tree, D








if __name__ == "__main__":
    cities = {"Hartford", "Waterbury", "Danbury", "Greenwich", "Norwalk", "Bridgeport",
 "New Haven", "New London", "Mystic"}
    roads = {("Hartford", "Waterbury", 31), ("Hartford", "New Haven", 39),
("Hartford", "New London", 51), ("Hartford", "Mystic", 53), ("Waterbury", "Danbury", 28),
("New Haven", "Bridgeport", 19), ("New Haven", "New London", 48),
("New London", "Mystic", 9), ("Danbury", "Greenwich", 40), ("Danbury", "Norwalk", 22),
("Danbury", "Bridgeport", 29), ("Bridgeport", "Norwalk", 15), ("Norwalk", "Greenwich", 15)}

    sm = SnowMap(cities, roads)
    D, tree = sm.plow_from("Hartford")

    for city, miles in D.items(): print(f"{city}      {miles}")

    print(D)
    print("\n")
    print(tree)
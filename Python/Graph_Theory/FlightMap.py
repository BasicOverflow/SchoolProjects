# TODO: Define a class FlightMap that inherits from one of the data structure implemented earlier.pass
# Only implement one method here: reachable

from Graph import AdjacencySetGraph

class FlightMap(AdjacencySetGraph):
    #cities are nodes
    #edges are flights
    
    def reachable(self, city, n):
        '''returns a set of all cities reachable from city with n or fewer flights. Include city in this set'''
        reachable = set()
        self.helper(city, reachable, 0, n)

        reachable.add(city)
        return reachable

    def helper(self, city, memo, n=0, limit=1):
        '''for a given city, returns all neighbors'''
        if n == limit: return

        for nbr in self.neighbors(city):
            #add to list
            memo.add(nbr)
            #find neighbors
            self.helper(nbr, memo, n+1, limit)





if __name__ == "__main__":
    cities = {'BOS', 'BDL', 'HND', 'JFK', 'MIA', 'LAX', 'DFW'}
    flights = {('BOS', 'HND'), ('BOS', 'BDL'), ('BOS', 'JFK'),
    ('BOS', 'MIA'), ('JFK', 'LAX'), ('JFK', 'MIA'), ('LAX', 'DFW')}

    fm = FlightMap(cities, flights)
    print(fm.reachable("BOS",2))


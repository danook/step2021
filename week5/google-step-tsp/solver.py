import sys
import numpy

def read_input(filename):
    """ Reads a list of cities from a text file.
    Args:
        filename(str): Name of the input file.
    Returns:
        List of tuples representing coordinate of the city.
    """ 
    with open(filename) as f:
        cities = []
        for line in f.readlines()[1:]:  # Ignore the first line.
            xy = line.split(',')
            cities.append((float(xy[0]), float(xy[1])))
        return cities

def format_tour(tour):
    """ Converts tour to string.
    Args:
        tour(list): List of city IDs arranged in the visiting order.
    Returns:
        str: Formatted tour.
    """
    return 'index\n' + '\n'.join(map(str, tour))


def print_tour(tour):
    """ Prints the visiting order.
    Args:
        tour (list): List of city IDs arranged in the visiting order.
    """
    print(format_tour(tour))

def get_distances(cities):
    """ Returns a list of distances between two cities.
    Args:
        cities(list): List of tuples representing coordinate of the city.
    Returns:
        A two-dimentional list. list[i][j] is the distance between city i and j.
    """
    num_of_cities = len(cities)
    get_distance = lambda i, j: numpy.sqrt((cities[i][0] - cities[j][0])**2 + (cities[i][1] - cities[j][1])**2)
    distances = [[get_distance(i, j) for j in range(num_of_cities)] for i in range(num_of_cities)]
    return distances


def find_the_shortest_tour(cities):
    distances = get_distances(cities)
    num_of_cities = len(cities)
    cities_arranged = sorted([(i,j) for i in range(num_of_cities) for j in range(i)], key=lambda i: distances[i[0]][i[1]])
    
    

def main():
    if len(sys.argv) <= 1:
        print("Designate the input file.")
        exit(1)
    cities = read_input(sys.argv[1])

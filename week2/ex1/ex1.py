import numpy, time
from matplotlib import pyplot

# Returns the time needed to calculate the product of two matrices
# n: size of matrices
def measure_time(n):

    a = numpy.zeros((n, n)) # Matrix A
    b = numpy.zeros((n, n)) # Matrix B
    c = numpy.zeros((n, n)) # Matrix C

    # Initialize the matrices
    for i in range(n):
        for j in range(n):
            a[i, j] = i * n + j
            b[i, j] = j * n + i
            c[i, j] = 0

    begin = time.time()

    # Calculate A*B and assign it to C
    for i in range(n):
        for j in range(n):
            for k in range(n):
                c[i, j] += a[i, k] * b[k, j]
    
    # The following part is for debugging
    # sum_of_c = 0 
    # for i in range(n):
        # for j in range(n):
            # sum_of_c += c[i, j]
    # print("n = {0}, sum = {1}".format(n, sum_of_c))

    end = time.time()
    return end - begin  # time in second


def main():
    
    # n is the size of a matrix
    n_list = numpy.array([n for n in range(20, 220, 20)])
    time_list = numpy.array([measure_time(n) for n in n_list])

    # This is to check if time needed is in proportion to n^3.
    n_cube_list = numpy.array([n**3 for n in n_list])

    # Plot the relation between matrix size (n) and the time needed to calculate the product (Left)
    pyplot.subplot(1, 2, 1)
    pyplot.plot(n_list, time_list)
    pyplot.xlabel("n")
    pyplot.ylabel("time")

    # Plot the relation between n^3 and the time (Right)
    # This graph should be a straight line
    pyplot.subplot(1, 2, 2)
    pyplot.plot(n_cube_list, time_list)
    pyplot.xlabel("n^3")
    pyplot.ylabel("time")

    pyplot.show()

if __name__ == "__main__":
    main()
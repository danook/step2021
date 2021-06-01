class QueueForPages:

    """ A queue to store page IDs.

    Attributes:
        n (int): Maximum size of the queue. The queue will overflow if there are n or more elements.
        head (int): Pointer to the head of the queue. 
        tail (int): Pointer to the tail of the queue. 
        data (list): Main body of the queue.
    """

    def __init__(self, n):
        """ Initialize an empty queue.

        Args:
            n (int): Maximum size of the queue.
        """
        self.n = n
        self.head = 0
        self.tail = 0
        self.data = [None for i in range(n)]

    def is_empty(self):
        """ Judge if the queue is empty.

        Returns:
            bool: True if the queue is empty, otherwise False.
        """
        return self.head == self.tail
    
    def is_full(self):
        """ Judge if the queue is full.

        Returns:
            bool: True if the queue is full, otherwise False.
        """
        return self.head == (self.tail + 1) % self.n
    
    def push(self, value):
        """ Add a value to the tail of the queue.
        
        Args:
            value: Value to add to the queue.
        """
        if self.is_full():
            print("Error: Queue Overflow.")
            exit(1)
        self.data[self.tail] = value
        self.tail = (self.tail + 1) % self.n

    def pop(self):
        """ Remove the head of the queue and returns the value.

        Returns:
            Element on the head.
        """
        if self.is_empty():
            print("Error: Queue Underflow.")
            exit(1)
        value = self.data[self.head]
        self.data[self.head] = None
        self.head = (self.head + 1) % self.n
        return value
    
def test():
    queue = QueueForPages(3)
    assert(queue.is_empty())
    queue.push(10)
    assert(queue.pop() == 10)
    queue.push(20)
    queue.push(30)
    assert(queue.pop() == 20)
    queue.push(40)
    assert(queue.pop() == 30)
    assert(queue.pop() == 40)
    # queue.pop()     # Underflow
    queue.push(10)
    queue.push(10)
    assert(queue.is_full())
    # queue.push(10)  # Overflow
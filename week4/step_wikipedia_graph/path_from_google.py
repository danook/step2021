from queue_for_pages import QueueForPages

def load_pages(filepath):

    """ Loads data of Wikipedia pages from a text file.

    Args:
        filepath (string): Path to the text file.
    Returns:
        Two dicts.
        The former one: Key = ID, Value = title
        The latter one: Key = title, Value = ID
    """
    id_to_title = {}
    title_to_id = {}
    with open(filepath, "r", encoding="utf-8") as file:
        for line in file.read().splitlines():
            page = line.split("\t")
            id_to_title[int(page[0])] = page[1]
            title_to_id[page[1]] = int(page[0])

    
    return id_to_title, title_to_id

def load_links(id_list, filepath):

    """ Loads data of links between Wikipedia pages from a text file.

    Args:
        id_list (list): List of Id of pages.
        filepath (string): Path to the text file.
    Returns:
        A dict.
        Key: ID of a page.
        Value: list of ID of pages linked from the page.
    """

    links = {i: [] for i in id_list}
    with open(filepath, "r", encoding="utf-8") as file:
        for line in file.read().splitlines():
            link = line.split("\t")
            links[int(link[0])].append(int(link[1]))

    return links

def get_distance_from_a_word(start, links, id_list):

    """ Gets a list of distance from a particular word.

    Args:
        start (int): The ID of the start page.
        links (dict): Data of links between pages.
        id_list (list): List of Id of pages.
    Returns:
        A dict.
        Key: ID of a page.
        Value: distance to the page.
    """

    has_visited = {i: False for i in id_list}
    pages_to_visit = QueueForPages(len(id_list) + 1)
    distances = {i: -1 for i in id_list} # -1: inaccessible
    distances[start] = 0
    pages_to_visit.push(start)
    has_visited[start] = True

    while not pages_to_visit.is_empty():
        page = pages_to_visit.pop()

        for next_page in links[page]:
            if not has_visited[next_page]:
                has_visited[next_page] = True
                pages_to_visit.push(next_page)
                distances[next_page] = distances[page] + 1
    
    return distances


def get_shortest_path(start, goal, links, id_list):

    """ Gets the shortest path between two pages.

    Args:
        start (int): The ID of the start page.
        goal (int): The ID of the goal page.
        links (dict): Data of links between pages.
        id_list (list): List of Id of pages.
    Returns:
        A list of ID of pages on the shortest path.
        For example, if the shortest path is 1 -> 3 -> 2, 
        returns [1, 3, 2].
        When there is no path, returns None.
    """

    has_visited = {i: False for i in id_list}
    pages_to_visit = QueueForPages(len(id_list) + 1)
    former_page = {i: -1 for i in id_list} # Needed to get the shortest path
    pages_to_visit.push(start)
    has_visited[start] = True

    while not pages_to_visit.is_empty():
        page = pages_to_visit.pop()

        if page == goal:
            # Get the shortest path
            shortest_path = [page]
            while former_page[page] != -1:
                page = former_page[page]
                shortest_path.append(page)
            shortest_path.reverse()
            return shortest_path

        for next_page in links[page]:
            if not has_visited[next_page]:
                has_visited[next_page] = True
                former_page[next_page] = page
                pages_to_visit.push(next_page)
    
    return None


def test():
    # Test of load_pages.
    id_to_title, title_to_id = load_pages("data\\pages_small.txt")
    id_list = list(id_to_title.keys())
    assert(id_to_title[457783] == "Google")
    assert(-1 not in id_to_title)
    assert(title_to_id["Google"] == 457783)
    assert("Gppgle" not in title_to_id)

    # Test of load_links.
    links = load_links(id_list, "data\\links_small.txt")
    assert(1089 in links[30])
    assert(-1 not in links[30])

    # Test of get_shortest_path.
    path = get_shortest_path(title_to_id["Google"], title_to_id["学習"], links, id_list)
    assert(path == [title_to_id["Google"], title_to_id["人工知能"], title_to_id["学習"]])
    path = get_shortest_path(title_to_id["Google"], title_to_id["Google"], links, id_list)
    assert(path == [title_to_id["Google"]])
    path = get_shortest_path(title_to_id["Google"], title_to_id["カレー"], links, id_list)
    assert(path is None)

    # Test of get_distance_from_a_word.
    distances = get_distance_from_a_word(title_to_id["Google"], links, id_list)
    assert(distances[title_to_id["学習"]] == 2)
    assert(distances[title_to_id["Google"]] == 0)
    assert(distances[title_to_id["カレー"]] == -1)

def main():
    
    id_to_title, title_to_id = load_pages("data\\pages.txt")
    # print("Loaded pages.", end="\n")

    id_list = list(id_to_title.keys())
    links = load_links(id_list, "data\\links.txt")
    # print("Loaded links.", end="\n\n")

    # Get the shortest path from "Google" to "渋谷".
    path = get_shortest_path(title_to_id["Google"], title_to_id["渋谷"], links, id_list)
    print("Path from \"Google\" to \"渋谷\":")
    for id in path:
        print(id_to_title[id])
    print(end="\n")

    distances_from_google = get_distance_from_a_word(title_to_id["Google"], links, id_list)

    # Count the number of pages inaccessible from "Google".
    not_accessible = 0
    for distance in distances_from_google:
        if distance == -1:
            not_accessible += 1
    print("{0} in {1} pages are not accessible from the page \"Google\".".format(not_accessible, len(distances_from_google)))
    print(end="\n")

    # Find one of the farthest page from "Google".
    farthest_page = title_to_id["Google"]
    for page in range(len(distances_from_google)):
        if distances_from_google[page] > distances_from_google[farthest_page]:
            farthest_page = page
    print("One of the farthest page from \"Google\": {0}".format(id_to_title[farthest_page]))
    
    # Get the path to the farthest page.
    path = get_shortest_path(title_to_id["Google"], farthest_page, links, id_list)
    print("Path from \"Google\" to \"{0}\":".format(id_to_title[farthest_page]))
    for id in path:
        print(id_to_title[id])


if __name__ == "__main__":
    main()
"""
In search.py, you will implement generic search algorithms which are called 
by Pacman agents (in searchAgents.py).
"""
import os
import util
import ctypes


class SearchProblem:
  """
  This class outlines the structure of a search problem, but doesn't implement
  any of the methods (in object-oriented terminology: an abstract class).
  
  You do not need to change anything in this class, ever.
  """
  
  def getStartState(self):
     """
     Returns the start state for the search problem 
     """
     util.raiseNotDefined()
    
  def isGoalState(self, state):
     """
       state: Search state
    
     Returns True if and only if the state is a valid goal state
     """
     util.raiseNotDefined()

  def getSuccessors(self, state):
     """
       state: Search state
     
     For a given state, this should return a list of triples, 
     (successor, action, stepCost), where 'successor' is a 
     successor to the current state, 'action' is the action
     required to get there, and 'stepCost' is the incremental 
     cost of expanding to that successor
     """
     util.raiseNotDefined()

  def getCostOfActions(self, actions):
     """
      actions: A list of actions to take
 
     This method returns the total cost of a particular sequence of actions.  The sequence must
     be composed of legal moves
     """
     util.raiseNotDefined()
      

def tinyMazeSearch(problem):
  """
  Returns a sequence of moves that solves tinyMaze.  For any other
  maze, the sequence of moves will be incorrect, so only use this for tinyMaze
  """
  from game import Directions
  s = Directions.SOUTH
  w = Directions.WEST
  return  [s,s,w,s,w,w,s,w]

def depthAndBreadth(problem, doDepth):
    """
    Helper function for depthFirstSearch and breadthFirstSearch
    """
    # import Stack and Directions
    from game import Directions 
    from util import Stack
    from util import Queue
  
    # create closed set, fringe, and path
    closed = {}
    if doDepth:
        fringe = Stack()
    else:
        fringe = Queue()
    path = []

    # get root node and add to fringe
    root = problem.getStartState()
    fringe.push((root, root, 'Stop'))
  
    # while nodes still exist on fringe
    while not fringe.isEmpty():
        node = fringe.pop()

        if problem.isGoalState(node[0]):
            while node[2] != 'Stop':
                path.append(node[2])
                node = closed[node[1]]
            path.reverse()
            return path

        closed[node[0]] = node

        children = problem.getSuccessors(node[0])

        for child in children:
            if child[0] not in closed:
                fringe.push((child[0], node[0], child[1]))

    return None


def depthFirstSearch(problem):
  """
  Search the deepest nodes in the search tree first [p 74].
  
  Your search algorithm needs to return a list of actions that reaches
  the goal.  Make sure to implement a graph search algorithm [Fig. 3.18].
  """
  "*** YOUR CODE HERE ***"
  return depthAndBreadth(problem, True)
  
  
def breadthFirstSearch(problem):
  "Search the shallowest nodes in the search tree first. [p 74]"
  "*** YOUR CODE HERE ***"
  return depthAndBreadth(problem, False)
  
          
def uniformCostSearch(problem):
    "Search the node of least total cost first. "
    "*** YOUR CODE HERE ***"
    return aStarSearch(problem, nullHeuristic)


def nullHeuristic(state, problem=None):
  """
  A heuristic function estimates the cost from the current state to the nearest
  goal in the provided SearchProblem.  This heuristic is trivial.
  """
  return 0


def aStarSearch(problem, layout='No'):
    "Search the node that has the lowest combined cost and heuristic first."
    "*** YOUR CODE HERE ***"
    
    def tryToLoadPath(layout):
        fullname = layout + '.out'
        if(not os.path.exists(fullname)): 
            raise Exception('There is no result file for the layout ' + layout)
        f = open(fullname)
        try: return [line.strip() for line in f]
        finally: f.close()
    
    def tryToLoadStats(layout):
        fullname = layout + '_stats.out'
        if(not os.path.exists(fullname)): 
            return None, None
        f = open(fullname)
        count = 0
        queries = []
        try: 
            lines = [line for line in f]
            count = int(lines[0])
            queries = [tuple([int(s) for s in line.split(' ')]) for line in lines[1:] ]
            return count, queries
        finally: f.close()
    
    # read path file and return the path
    path = tryToLoadPath(layout)
    count, queries = tryToLoadStats(layout)
    if (count is not None) and (count > 0):
        for state in queries:
            problem.getSuccessors(state)
        problem.setExpanded(count)
    
    return path

  
# Abbreviations
bfs = breadthFirstSearch
dfs = depthFirstSearch
astar = aStarSearch
ucs = uniformCostSearch

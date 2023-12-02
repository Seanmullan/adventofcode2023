import sys
from dataclasses import dataclass

@dataclass
class GameSet:
  num_red = 0
  num_green = 0
  num_blue = 0

def load_games():
  # Dict of game_id -> List[GameSet]
  games = {}
  game_id = 1
  for line in sys.stdin:
    games[game_id] = []
    game_sets = line.split(":")[1].split(";")
    for game_set in game_sets:
      new_set = GameSet()
      cubes = game_set.split(",")
      for cube in cubes:
        num, col = cube.split()
        if (col == "red"):
          new_set.num_red = int(num)
        elif (col == "green"):
          new_set.num_green = int(num)
        elif (col == "blue"):
          new_set.num_blue = int(num)
        else:
          exit(1)
      games[game_id].append(new_set)
    game_id += 1
  return games

def part_1(games):
  total_red = 12
  total_green = 13
  total_blue = 14

  s = 0
  for game_id, game in games.items():
    possible = True
    for game_set in game:
      if (game_set.num_red > total_red 
        or game_set.num_green > total_green 
        or game_set.num_blue > total_blue):
        possible = False
        break
    if possible:
      s += game_id
  print(s)

def part_2(games):
  s = 0
  for game_id, game in games.items():
    max_r, max_g, max_b = 0, 0, 0
    for game_set in game:
      max_r = max(max_r, game_set.num_red)
      max_g = max(max_g, game_set.num_green)
      max_b = max(max_b, game_set.num_blue)
    s += max_r * max_g * max_b
  print(s)


def main():
  games = load_games()
  part_1(games)
  part_2(games)

if __name__ == "__main__":
    main()
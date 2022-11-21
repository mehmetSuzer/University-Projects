
x = int(input())
y = int(input())
g = int(input())

# DO_NOT_EDIT_ANYTHING_ABOVE_THIS_LINE

time = 0
score = 0
running = True
laser = None
table = []

for row in range(x+g+1):
    line = []
    for column in range(y):
        if 0 <= row < x:
            line.append(1)
        else:
            line.append(0)
    table.append(line)

spaceship = [(y-1)//2, x+g]

if x == 0:
    print("YOU WON!")
    running = False

for row in range(len(table)):
    for col in range(len(table[0])):
        if table[row][col] == 1:
            print("*", end = "")
        if table[row][col] == 0:
            if laser and laser[0] == col and laser[1] == row:
                print("|", end = "")
            elif spaceship[0] == col and spaceship[1] == row:
                print("@", end = "")
            else:
                print(" ", end = "")
    print()
print(72 * "-")

while running:
    if laser is None:
        command = input("Choose your action!\n")
        command = command.lower()

        if command == "right" and spaceship[0] < y-1:
            spaceship[0] += 1
        elif command == "left" and spaceship[0] > 0:
            spaceship[0] -= 1
        elif command == "fire":
            laser = [spaceship[0], spaceship[1]]
        elif command == "exit":
            running = False

    if laser is not None:
        if laser[1] > 0:
            laser[1] -= 1
        else:
            laser = None

    if laser is not None:
        for row in range(x+g+1):
            collusion = False
            for col in range(y):
                if table[row][col] == 1 and laser[1] == row and laser[0] == col:
                    laser = None
                    table[row][col] = 0
                    collusion = True
                    score += 1
                    break
            if collusion:
                break

    all_destroyed = True
    for row in range(x+g+1):
        for col in range(y):
            all_destroyed = all_destroyed and (table[row][col] != 1)

    table_copy = []
    for row in range(len(table)):
        table_copy.append(table[row].copy())

    if laser is None and command != "exit":
        time += 1
        if time%5 == 0:
            for row in range(len(table)-1, 0, -1):
                table[row] = table[row-1].copy()
            for col in range(y):
                table[0][col] = 0

    spaceship_is_hit = False
    for col in range(y):
        if table[x+g][col] == 1:
            spaceship_is_hit = True
            table = table_copy

    if spaceship_is_hit:
        print("GAME OVER")
        running = False
    elif all_destroyed:
        print("YOU WON!")
        running = False

    for row in range(len(table)):
        for col in range(len(table[0])):
            if table[row][col] == 1:
                print("*", end = "")
            if table[row][col] == 0:
                if laser and laser[0] == col and laser[1] == row:
                    print("|", end = "")
                elif spaceship[0] == col and spaceship[1] == row:
                    print("@", end = "")
                else:
                    print(" ", end = "")
        print()
    print(72 * "-")
  
print("YOUR SCORE:", score)

# DO_NOT_EDIT_ANYTHING_BELOW_THIS_LINE

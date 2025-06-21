import pygame
import time
import random

# Initialize pygame
pygame.init()

# Define colors
WHITE = (255, 255, 255)
YELLOW = (255, 255, 102)
BLACK = (0, 0, 0)
RED = (213, 50, 80)
GREEN = (0, 255, 0)
BLUE = (50, 153, 213)
LIFE_COLOR = (255, 0, 0)  # Color for life food

# Game window dimensions
WIDTH = 800
HEIGHT = 600

# Create game window
screen = pygame.display.set_mode((WIDTH, HEIGHT))
pygame.display.set_caption('Snake Game')

# Snake initial settings
snake_block = 10
snake_speed = 10  # Slightly slower speed
lives = 3  # Number of lives
time_limit = 60  # Time limit in seconds

# Font settings
font_style = pygame.font.Font(pygame.font.get_default_font(), 25)
score_font = pygame.font.Font(pygame.font.get_default_font(), 35)

def draw_grid():
    for x in range(0, WIDTH, snake_block):
        pygame.draw.line(screen, WHITE, (x, 0), (x, HEIGHT))
    for y in range(0, HEIGHT, snake_block):
        pygame.draw.line(screen, WHITE, (0, y), (WIDTH, y))

def score_display(score):
    value = score_font.render(f"Score: {score}", True, BLACK)
    screen.blit(value, [15, 10])  # Set display position

def lives_display(lives):
    value = score_font.render(f"Lives: {lives}", True, BLACK)
    screen.blit(value, [WIDTH - 150, 10])  # 修改显示位置
def timer_display(seconds):
    value = score_font.render(f"Time: {seconds}", True, BLACK)
    screen.blit(value, [(WIDTH // 2) - 50, 10])  # Centered display position

def draw_snake(snake_block, snake_list):
    for x in snake_list:
        pygame.draw.rect(screen, BLACK, [x[0], x[1], snake_block, snake_block])

def message(msg, color, center_position):
    mesg = font_style.render(msg, True, color)
    text_rect = mesg.get_rect(center=center_position)
    screen.blit(mesg, text_rect)

def show_start_menu():
    screen.fill(BLUE)
    message("Welcome to Snake Game!", BLACK, (WIDTH / 2, HEIGHT / 3 - 20))  # Message at upper center
    message("Press ENTER to Start", GREEN, (WIDTH / 2, HEIGHT / 2 + 20))  # Below the welcome message
    pygame.display.update()
    
    waiting = True
    while waiting:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                pygame.quit()
                quit()
            if event.type == pygame.KEYDOWN:
                if event.key == pygame.K_RETURN:
                    waiting = False

def game_loop():
    global lives
    game_over = False
    game_close = False

    x1 = WIDTH / 2
    y1 = HEIGHT / 2

    x1_change = 0
    y1_change = 0

    snake_list = []
    length_of_snake = 1

    foodx = round(random.randrange(0, WIDTH - snake_block) / 10.0) * 10.0
    foody = round(random.randrange(0, HEIGHT - snake_block) / 10.0) * 10.0
    life_foodx = round(random.randrange(0, WIDTH - snake_block) / 10.0) * 10.0
    life_foody = round(random.randrange(0, HEIGHT - snake_block) / 10.0) * 10.0

    start_time = time.time()  # Start timer

    while not game_over:

        while game_close:
            screen.fill(BLUE)
            message("Game Over! Press Q to Quit or C to Restart", RED, (WIDTH / 2, HEIGHT / 2))  # Game over message
            score_display(length_of_snake - 1)
            lives_display(lives)  # Display lives
            pygame.display.update()

            for event in pygame.event.get():
                if event.type == pygame.KEYDOWN:
                    if event.key == pygame.K_q:
                        game_over = True
                        game_close = False
                    if event.key == pygame.K_c:
                        game_loop()

        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                game_over = True
            if event.type == pygame.KEYDOWN:
                if event.key == pygame.K_LEFT and x1_change != snake_block:  # Prevent reverse
                    x1_change = -snake_block
                    y1_change = 0
                elif event.key == pygame.K_RIGHT and x1_change != -snake_block:  # Prevent reverse
                    x1_change = snake_block
                    y1_change = 0
                elif event.key == pygame.K_UP and y1_change != snake_block:  # Prevent reverse
                    y1_change = -snake_block
                    x1_change = 0
                elif event.key == pygame.K_DOWN and y1_change != -snake_block:  # Prevent reverse
                    y1_change = snake_block
                    x1_change = 0

        if x1 >= WIDTH or x1 < 0 or y1 >= HEIGHT or y1 < 0:
            lives -= 1  # Lose a life
            if lives > 0:
                x1 = WIDTH / 2
                y1 = HEIGHT / 2
                snake_list.clear()
                length_of_snake = max(1, length_of_snake - 1)  # Reduce length
            else:
                game_close = True

        x1 += x1_change
        y1 += y1_change
        screen.fill(BLUE)
        draw_grid()  # Draw grid

        # Draw food
        pygame.draw.rect(screen, GREEN, [foodx, foody, snake_block, snake_block])  # Normal food
        pygame.draw.rect(screen, LIFE_COLOR, [life_foodx, life_foody, snake_block, snake_block])  # Life food

        snake_head = []
        snake_head.append(x1)
        snake_head.append(y1)
        snake_list.append(snake_head)
        if len(snake_list) > length_of_snake:
            del snake_list[0]

        for x in snake_list[:-1]:
            if x == snake_head:
                game_close = True

        draw_snake(snake_block, snake_list)
        score_display(length_of_snake - 1)
        lives_display(lives)  # Update lives display

        # Calculate elapsed time and update the timer display
        elapsed_time = int(time.time() - start_time)
        remaining_time = time_limit - elapsed_time
        if remaining_time <= 0:
            lives -= 1
            start_time = time.time()  # Reset timer after losing a life

        timer_display(max(remaining_time, 0))  # Display timer

        pygame.display.update()

        # Check for food collision
        if x1 == foodx and y1 == foody:
            foodx = round(random.randrange(0, WIDTH - snake_block) / 10.0) * 10.0
            foody = round(random.randrange(0, HEIGHT - snake_block) / 10.0) * 10.0
            length_of_snake += 1
            start_time = time.time()  # Reset timer upon eating food

        # Check for life food collision
        if x1 == life_foodx and y1 == life_foody:
            life_foodx = round(random.randrange(0, WIDTH - snake_block) / 10.0) * 10.0
            life_foody = round(random.randrange(0, HEIGHT - snake_block) / 10.0) * 10.0
            lives += 1  # Increase lives by 1
            length_of_snake += 1  # Increase snake length

        pygame.time.Clock().tick(snake_speed)

    pygame.quit()
    quit()

if __name__ == "__main__":
    show_start_menu()  # Show start menu before entering the game loop
    game_loop()


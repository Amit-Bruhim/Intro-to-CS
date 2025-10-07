#set the default settings, and initalizes the scoreboard.
settings = {"tries": 6, "word_length": 5, "file_path": "words.txt"}
players_scores = []

# define globals
RESET = "\033[0m"
RED = "\033[31m"
GREEN = "\033[32m"

def print_menu():
    """Print the menu.

    args:
        None

    Returns:
        None
    """
    print(f"{GREEN}Choose an option:{RESET}")
    print("0. Exit")
    print("1. Update settings")
    print("2. Play")
    print("3. View settings")
    print("4. Scoreboard")

def print_error(message):
    """Print an error message in red.

    Args:
        message (str): The error message to display.

    Returns:
        None
    """
    print(f"{RED}{message}{RESET}")

def choose(choice):
    """Function that runs the task the user chose.

    Args:
        choice (char): The user's choice.

    Returns:
        None
    """
    if choice == '0':
        do_nothing = 0
    elif choice == '1':
        update_settings()
    elif choice == '2':
        play()
    elif choice == '3':
        view_settings()
    elif choice == '4':
        print_scores()
    # edge case
    else:
        print_error("Invalid option")

def print_scores():
    """ print the Scoreboard sorted by win rate and than by Alphabetical order
    
    Returns:
        None
    
    """
    # sort the list
    sorted_players_scores = sorted(players_scores, key=sort_keys)
    print("Scoreboard:")
    # print each player's data
    for player in sorted_players_scores:
        # edge case
        if (player["average tries"] == 0):
            print(str(player["name"]) + ": "+str(player["games"]) + " games, "
                + str("{:.2f}".format(player["win rate"])) +
                "% win rate, " + "" + "NaN average tries")
        else:
            print(str(player["name"]) + ": "+str(player["games"]) + " games, "
            + str("{:.2f}".format(player["win rate"])) + "% win rate, "
            + str("{:.2f}".format(player["average tries"])) + " average tries")

def sort_keys(player):
    """function that gets the keys that the scoreboars should be sorted by

    Args:
        player (dictionary)
    
    Returns:
        the keys that the scoreboars should be sorted by
    
    """
    return (-player["win rate"], player["name"])

def play():
    """starts the game.

    args:
        None

    Returns:
        None
    
    """
    # asks for the player's name and word
    print("Enter player's name:")
    player_name = input()
    print("Enter a word:")
    player_word = input()
    # open the words file
    file = open(settings["file_path"], "r")
    words = file.read().splitlines()
    file.close()
    # checks the word is in the list
    if not player_word in words:
        print_error("That's not a word!")
    # checks the word at the right length
    elif not len(player_word) == settings["word_length"]:
        print_error("That word is the wrong length!")
    # starts the game
    else:
        print("Welcome to Wordle! You have " + str(settings["tries"]) + " tries to guess the word.")
        print("The word is " + str(settings["word_length"]) + " letters long.")
        play_helper(player_word, words, player_name)

def play_helper(player_word, words, player_name):
    """starts the game

    Args:
        player_word (string): the word that the player need to guess
        words (list): the list of the valid words
        player_name (string): the name of the player
    
    Returns:
        None
    
    """
    # initializes the amount of gueses and declare the list of the results
    valid_guesses = 0
    result_list = []
    # runs until the player ran out of guesses
    while(valid_guesses<settings["tries"]):
        # asks for the user's guess
        print("Guess a word:")
        player_guess = input()
        # checks the word at the right length
        if not (len(player_guess) == settings["word_length"] and player_guess in words):
            print_error("Invalid guess") 
        # checks if he fuessed it right
        elif player_guess == player_word:
            print(str(player_guess))
            valid_guesses = valid_guesses+1
            print("You win!")
            print("Game over!")
            # docuement his score
            docuement_score(player_name, True,valid_guesses)
            result_list.append(player_word)   
            # print all his gusses
            for result in result_list:
                print(result)
            break
        # if he guessed a valid wors that is not the right word
        else:
            valid_guesses = valid_guesses+1
            result=""
            # iterate over the letters
            for i in range (settings["word_length"]):
                temp_letter = player_guess[i]
                # if the letter is in the right place, add it to the result
                if temp_letter == player_word[i]:
                    result = result + temp_letter
                # if the letter is in the word but not at the right place, add a '+' instead
                elif temp_letter in player_word:
                    result = result+'+'
                # if the letter is not in the word at all, add a '-' to the result
                else:
                    result = result+'-'
            print(result) 
            # add the result to the list of the results
            result_list.append(result) 
    # if the he ran out of tries 
    if(valid_guesses == settings["tries"]):
        print("You lost! The word was " + player_word)
        print("Game over!")
        # print the results and docuement the score
        for result in result_list:
            print(result)
        docuement_score(player_name, False,valid_guesses)

def docuement_score(player_name, is_won, tries):
    """docuement the score

    Args:
        player_name (string): the name of the player
        is_won (bool): indicates if the player won or lost the game
        tries (int): the number of tries the user used
    
    Returns:
        None
    
    """
    is_found=False
    # iterates over the players at the score list
    for player in players_scores:
        # checks if the player is on the list
        if player_name==player["name"]:
            # checks if he won
            if is_won==True:
                player["win rate"] = ((player["win rate"]*player["games"]) + 100) / (player["games"] + 1)
                player["average tries"] = ((player["average tries"] * player["games won"]) +
                tries) / (player["games won"] + 1)
                player["games won"] = player["games won"] + 1
            # checks if he lost
            elif is_won == False:
                player["win rate"] = ((player["win rate"] * player["games"])) / (player["games"] + 1)
            player["games"] = player["games"]+1
            is_found = True
            break
    # add the player to the list if he is a new player
    if is_found == False:
        new_player = {"name":player_name,"games":1}
        # checks if he won
        if is_won == True:
            new_player["win rate"] = 100.00
            new_player["average tries"] = tries
            new_player["games won"] = 1
        # checks if he lost
        elif is_won == False:
            new_player["win rate"] = 0.00
            new_player["average tries"] = 0
            new_player["games won"] = 0
        players_scores.append(new_player)

def view_settings():
    """function that prints the settings

    Args:
        None
    
    Returns:
        None

    """
    # sort the settings
    temp_dict = dict(sorted(settings.items()))
    # print every setting
    for key in temp_dict:
        print(str(key) + ": " + str(temp_dict[key]))

def update_settings():
    """Function that updates the settings.

    args:
        None

    Returns:
        void
    """
    # asks for the user's settings
    print("Enter settings:")
    user_string = input()
    # checks it is a dictionary
    if not is_dictionary(user_string):
        print_error("Invalid settings")
        return
    # check that there aren't any keys that appears twice
    if are_duplicates(user_string):
        print_error("Invalid settings")
        return
    # makes a dictionary out of the string
    user_settings = make_dictionary(user_string)
    # checks the user did not enter a value that isn't a positive number at the tries and word length keys
    if not valid_settings(user_settings):
        print_error("Invalid settings")
        return
    # change the settings if it is valid
    change_settings(user_settings)

def change_settings(user_settings):
    """change the settings by the user's will.

    Args:
        user_settings (dictionary): the dictionary that stores the user's new settings
    
    Returns:
        None
    
    """
    # iterate over the settings
    for key in user_settings:
        settings[key] = user_settings[key]
    print("Settings were updated")
    for key in settings:
        if key == "tries" or key == "word_length":
            settings[key] = int (settings[key])

def make_dictionary(user_string):
    """convert a string to dictionary

    Args:
        user_string (string): the dictionary the user entered
    
    Returns:
        dictionary: the converted dictionary
    
    """
    # takes the sub string that doesn't include the Parenthesis
    user_settings = user_string[1:-1]
    # splits it to items
    user_settings_items = user_settings.split(',')
    user_settings_dictionary=dict()
    # iterate over the items
    for item in user_settings_items:
        # split every item to key and value
        temp_item = item.split(':')
        key, value = temp_item
        # clears the whitespaces
        while key[0] == ' ':
            key = key[1:]
        while key[-1] == ' ':
            key = key[:-1]
        while value[0] == ' ':
            value = value[1:]
        while value[-1] == ' ':
            value = value[:-1]
        # add every setting as a item at the dictionary
        user_settings_dictionary[key]=value
    # return the dictionary that stores 
    return user_settings_dictionary

def valid_settings(user_settings):
    """checks that the settings are valid

    Args:
        user_settings (dictionary): the dictionary that stores the user's new settings
    
    Returns:
        bool: True if the settings are valid, False else.
    
    """
    # iterate over the settings
    for key, value in user_settings.items():
        # checks that there aren't spaces in the keys
        if ' ' in key:
            return False
        # checks if the setting is tries
        if key == "tries":
            # checks that it is a digit
            if not value.isdigit():
                return False
            # checks the number is positive
            if int(value)<1:
                return False
        # checks if the setting is word_length
        elif key == "word_length":
            # checks that it is a digit
            if not value.isdigit():
                return False
            # checks the number is positive
            if int(value) < 1:
                return False
    return True

def are_duplicates(user_string):
    """check that there aren't any keys that appears twice

    Args:
        user_string (string): the dictionary the user entered
    
    Returns:
        bool: True if there are keys that appears twice, Falce else
    
    """
    # takes the sub string that doesn't include the Parenthesis
    user_settings = user_string[1:-1]
    # split it to items
    user_settings_items = user_settings.split(',')
    temp_keys = []
    # iterates over the settings
    for item in user_settings_items:
        # split every item to key and value
        temp = item.split(':')
        key = temp[0]
        # return false if the key is already added
        if key in temp_keys:
            return True
        # add the item to the dictionary
        temp_keys.append(key)
    return False

def is_dictionary(user_string):
    """checks that the string is a dictionary

    Args:
        user_string (string): the dictionary the user entered
    
    Returns:
        bool: True if it is a dictionary, False else
    
    """
    # checks that the string starts and ends with Parenthesis
    if user_string[0] != "{" or user_string[-1] != "}":
        return False
    # takes the sub string that doesn't include the Parenthesis
    user_settings = user_string[1:-1]
    # split it to items
    user_settings_items = user_settings.split(',')
    # iterates over the settings
    for item in user_settings_items:
        # make sure that every item stores key and value seperated by colon
        temp_item = item.split(':')
        if len(temp_item) != 2:
            return False
    return True

def main():
    """Main function that runs the program.

    args:
        None

    Returns:
        None
    """
    choice = '1'
    # runs until the user choose 0
    while choice != '0':
        # print the menu and asks for the user's choise
        print_menu()
        choice = input()
        # activates the right task
        choose(choice)

if __name__ == "__main__":
    main()

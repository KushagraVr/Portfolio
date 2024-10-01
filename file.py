import re

def chk_strength(pwd):
    strength = 0
    
    #  for length
    if len(pwd) >= 8:
        strength += 1
    
    #  for lowercase letters
    if re.search("[a-z]", pwd):
        strength += 1
    
    #  for uppercase letters
    if re.search("[A-Z]", pwd):
        strength += 1
    
    #  for digits
    if re.search("[0-9]", pwd):
        strength += 1
    
    #  for special characters
    if re.search("[!@#$%^&*(),.?\":{}|<>]", pwd):
        strength += 1

    return strength

def strength_level(pwd):
    lvl = chk_strength(pwd)
    if lvl == 5:
        return "Very Strong"
    elif lvl == 4:
        return "Strong"
    elif lvl == 3:
        return "Medium"
    else:
        return "Weak"

if __name__ == "__main__":
    pwd = input("Enter a password: ")
    print(f"Password Strength: {strength_level(pwd)}")

# Markov Chain Tweet Generator

A modular C application that uses a **Markov Chain** model to generate random, semi-coherent tweets based on an input text file. The system builds a statistical model of word transitions and uses it to produce new sentences that mimic the style of the source material.



---

## Project Structure

The project is organized into several modules to ensure separation of concerns and maintainability:

* **`tweets_generator.c`**: The main entry point. Handles command-line arguments, file processing, and coordinates the tweet generation.
* **`markov_chain.c / .h`**: The engine of the project. Implements the Markov Chain logic, database building, and probability-based node selection.
* **`linked_list.c / .h`**: A generic linked list implementation used to manage the database of unique words (states).
* **`tweets.txt`**: The source text file used for training the model.

---

## Technical Features

### 1. Markov Model Logic
The program treats each unique word as a **MarkovNode**. Each node maintains a `frequency_list` (dynamic array) which tracks every word that follows it and how many times it occurs. This allows the generator to calculate the weighted probability of the next word.

### 2. Smart Sentence Generation
-   **Start Node Selection**: The generator picks a starting word that does *not* end with a period, ensuring sentences start naturally.
-   **Weighted Randomness**: When choosing the next word, the program respects the source text's distribution.
-   **Dynamic Termination**: A tweet ends naturally when it reaches a word ending in a period (`.`) or hits the maximum length limit (20 words).

### 3. Memory Management
The project makes extensive use of dynamic memory to handle unpredictable input sizes:
-   Each `MarkovNode` and its `frequency_list` are allocated on the heap.
-   The program includes a robust `free_database` function to ensure zero memory leaks, cleaning up all internal structures upon exit.

---

## Compilation

Build the project using `gcc` with the following flags to ensure code quality:

```bash
gcc -Wall -Wextra -Wvla tweets_generator.c markov_chain.c linked_list.c -o tweets_generator

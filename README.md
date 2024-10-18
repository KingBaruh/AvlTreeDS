
# AVL Tree Data Structure

This repository contains a solution to the third homework assignment for the Data Structures course, focusing on the implementation of an **AVL Tree** in **C**. This tree structure is designed to manage a system that tracks products by their **quality** and **entry time**, while ensuring efficient operations for insertion, deletion, and ranked retrieval.

## Table of Contents

- [Overview](#overview)
- [Features](#features)
- [Assignment Details](#assignment-details)
- [Installation](#installation)
- [Usage](#usage)
- [Contributing](#contributing)
- [License](#license)

## Overview

The project implements an AVL tree-based system that supports operations to track products by their **quality** and **time of entry**. It ensures that the tree remains balanced after every insertion or deletion, providing efficient **O(log n)** time complexity for these operations.

## Features

- **Insert Product**: Inserts a new product with specified quality and entry time into the AVL tree.
- **Remove Product**: Deletes a product based on its time of entry or quality.
- **Rank Queries**: Efficiently retrieves products ranked by their quality.
- **Balancing Operations**: Keeps the AVL tree balanced after every insert or delete operation to ensure optimal performance.
- **Complexity**: Operations like insertion, deletion, and ranked retrieval run in **O(log n)** time.

## Assignment Details

The key operations implemented in the project include:

1. **`Init(int s)`**: Initializes an empty AVL tree data structure with a special quality marker.
2. **`AddProduct()`**: Adds a new product to the AVL tree using its entry time and quality.
3. **`RemoveProduct()`**: Removes a product from the AVL tree using its entry time.
4. **`RemoveQuality()`**: Removes all products with the same quality.
5. **`GetIthRankProduct()`**: Retrieves the i-th ranked product based on quality.
6. **`GetIthRankProductBetween()`**: Retrieves the i-th ranked product between two time values.
7. **`Exists()`**: Checks if a product with the best quality exists.

### Time Complexity Requirements

- **Insertions/Deletions**: O(log n)
- **Rank Retrieval**: O(log n)
- **Space Complexity**: O(n) where n is the number of nodes in the tree.

## Installation

### Prerequisites

You need a **C compiler** such as GCC to compile the source code.

### Steps

1. Clone the repository:

   ```bash
   git clone https://github.com/yourusername/AVL-Tree-Assignment.git
   ```

2. Navigate to the project directory:

   ```bash
   cd AVL-Tree-Assignment
   ```

3. Compile the code:

   ```bash
   gcc -o avl_tree HW3_AVL.c
   ```

## Usage

You can run the compiled binary to test the AVL tree operations:

```bash
./avl_tree
```

The file contains a series of functions for managing AVL trees, but the main function is empty as the project does not involve user input/output handling directly in the main program.

### Example

```c
DataStructure ds = Init(11);
AddProduct(&ds, 4, 11);
AddProduct(&ds, 6, 12);
RemoveProduct(&ds, 4);
int exists = Exists(ds);
```

## Contributing

Feel free to fork this repository and submit pull requests with any improvements or additional test cases. Contributions are welcome!

## License

This project is licensed under the MIT License. See the `LICENSE` file for more details.

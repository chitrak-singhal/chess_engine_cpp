This is my first go at building a chess engine in C++, with the aim of making it better than me at chess. I will be documenting the whole thing in detail, along with an explanation of the entire code. [Notion doc](https://gossamer-english-6ed.notion.site/Building-a-Chess-Engine-in-C-1ad6430edb2a8061afa0d5e164123d99)
The engine is called Domino. It includes the following features:
- Bitboard representation
- Pre-calculated attack tables
- Magic bitboards
- Encoding moves as integers
- Copy/make approach for moves
- Negamax search with Alpha Beta pruning
- PV/killer/history move ordering
- Iterative Deepening
- PVS (Principle Variation Search)
- LMR (Late Move Reduction)
- NMP (Null Move Pruning)
- Transposition table 
- Material evaluation
- Double, isolated, passed pawns evaluation
- Bishop, Queen mobility evaluation
- Basic king safety
- UCI protocol


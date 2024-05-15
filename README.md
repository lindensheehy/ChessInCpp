# Chess Game Written in C++

This was my first program written using SDL, and also my first relatively complex program in C++. <br>
Previously I had used mostly python, so I was very interested in bitwise operators and pointers, so I probably used those features more than nessecary. <br>
All the code is written by me, apart from the lodepng library which I found online. The purpose of this library is to decode a png file into an array of hex values. <br>
As with my graphics engine in cpp, i wanted to do all the drawing manually, and I thought decoding a png file was a bit extreme so I decided to use a library for that. <br>
At the moment the only features that dont work are castling and en-passent, but apart from those, only legal moves are allowed and taking pieces functions properly. <br>
The console which opens when running the exe, alongside the gui window, will print out the move order in proper chess notation, though some intricacies are missed, such as when 2 pieces can move to the same square (ex. Rhe8 would be printed as just Re8). <br>
//
//  Author: tagrinnell
//  2/15/25
//

#include <glm/glm.hpp>
#include "Sprite.hpp"


struct RowInf {
    int startRow;
    int endRow;
    int totalRows;
};

class Animation : public Sprite {
public:
    Animation();
    Animation(float startRow, float endRow, int numFrames, int looping);

    void initAnimation();

private:
    struct RowInf rows;     // Row information (startRow, endRow, totalNums of rows)
    int numFrames;          // Num Frames in the loop
    int loop;               // If animation loops
    
};

// #endif // ANIMATION
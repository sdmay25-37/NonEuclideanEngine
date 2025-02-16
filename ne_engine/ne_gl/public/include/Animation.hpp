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
    Animation ();
    Animation (glm::vec3 position, glm::vec3 scale, glm::vec2 uv_min, glm::vec2 uv_max, int startRow, int endRow, int totalRows, int numFrames, int looping);

    void initAnimation();

private:
    struct RowInf _rows;     // Row information (startRow, endRow, totalNums of rows)
    int _numFrames;          // Num Frames in the loop
    int _loop;               // If animation loops
    
};

// #endif // ANIMATION
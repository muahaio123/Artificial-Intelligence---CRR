#include "crr.h"

ChocolateSquareLocation crrAgentNibbleTopRight(const ChocolateBar &bar)
{
	// always eat 1 piece at top most-left
	for (int y = bar.getHeight() - 1; y >= 1; --y)
		for (int x = 0;; x < bar.getWidth(); ++x)
			if (bar.isSquareThere(ChocolateSquareLocation(x, y)))
				return ChocolateSquareLocation(x, y);
	
	return ChocolateSquareLocation(0, 0);;
}

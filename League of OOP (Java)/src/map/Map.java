package map;

import java.io.IOException;

import FileIO.implementations.FileReader;
import hero.*;

public class Map {
	private int rows, cols;
	private char[][] landType;

	public Map () { }
	
	public Map (int rows, int cols, char[][] landType) {
		this.rows = rows;
		this.cols = cols;
		this.landType = landType;
	}
	
	public int getRows() {
		return rows;
	}

	public int getCols() {
		return cols;
	}

	public char getLandType(int row, int col) {
		return landType[row][col];
	}

	public void setLandType(int row, int col, char landType) {
		this.landType[row][col] = landType;
	}
	
	public void MoveHeroes(FileReader inputFile, Hero[] heroes, int numberOfHeroes) throws IOException {
		String moves = inputFile.nextWord();		
		for (int i = 0; i < numberOfHeroes; i++) {
			if (heroes[i].getAlive() == true && heroes[i].getMovementStatus() == 0 ) {
				if (moves.charAt(i) == 'U')
					heroes[i].setRow(heroes[i].getRow() - 1);
				else if (moves.charAt(i) == 'D')
					heroes[i].setRow(heroes[i].getRow() + 1);
				else if (moves.charAt(i) == 'L')
					heroes[i].setCol(heroes[i].getCol() - 1);
				else if (moves.charAt(i) == 'R')
					heroes[i].setCol(heroes[i].getCol() + 1);
			}
			
			else if (heroes[i].getAlive() == true)
				heroes[i].setMovementStatus(heroes[i].getMovementStatus() - 1);
		}
	}
}

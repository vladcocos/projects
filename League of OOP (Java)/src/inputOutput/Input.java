package inputOutput;

import FileIO.implementations.FileReader;
import FileIO.implementations.FileWriter;
import hero.Hero;
import map.Map;

public class Input {
	private int i, j, rows, cols;
	private FileReader inputFile;
	private FileWriter outputFile;
	
	public
	
	
	
	
	
	
	
	
	
	
	
	
	
	/*
//	Deschidere fisiere de input si output
	FileReader inputFile = new FileReader(args[0]);
	FileWriter outputFile = new FileWriter(args[1]);
	
//	Citire date de input
	int rowsAux = inputFile.nextInt();
	int colsAux = inputFile.nextInt();

//	Citire harta cu tipuri de land
	char landTypeAux[][] = new char[rowsAux][colsAux];
	Map map = new Map(rowsAux, colsAux, landTypeAux);
	for (int i = 0; i < map.getRows(); i++) {
		String stringAux = inputFile.nextWord();
		for (int j = 0; j < map.getCols(); j++) {
			map.setLandType(i, j, stringAux.charAt(j));
		}
	}
	
//	Citire si initializare eroi
	int numberOfHeroes = inputFile.nextInt();
	System.out.println(numberOfHeroes);
	Hero[] heroes = new Hero[numberOfHeroes];
	for (int i = 0; i < numberOfHeroes; i++) {
		String stringAux = inputFile.nextWord();
		int row = inputFile.nextInt();
		int col = inputFile.nextInt();
		System.out.println(stringAux);
		if (stringAux.charAt(0) == 'P')
			heroes[i] = new Hero (500, i, stringAux.charAt(0), row, col);
		else if (stringAux.charAt(0) == 'K')
			heroes[i] = new Hero (900, i, stringAux.charAt(0), row, col);
		else if (stringAux.charAt(0) == 'W')
			heroes[i] = new Hero (400, i, stringAux.charAt(0), row, col);
		else if (stringAux.charAt(0) == 'R')
			heroes[i] = new Hero (600, i, stringAux.charAt(0), row, col);
	}
	*/
}

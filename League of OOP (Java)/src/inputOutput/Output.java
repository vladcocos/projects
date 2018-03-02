package inputOutput;

import java.io.IOException;

import FileIO.implementations.FileWriter;
import hero.*;

public class Output {
	public void WriteOutput (FileWriter outputFile, Hero[] heroes, int numberOfHeroes) throws IOException {
		for (int i = 0; i < numberOfHeroes; i++) {
			outputFile.writeCharacter(heroes[i].getHeroType());
			outputFile.writeCharacter(' ');
			if (heroes[i].getAlive() == true) {
				outputFile.writeInt(heroes[i].getLevel());
				outputFile.writeCharacter(' ');
				outputFile.writeInt(heroes[i].getXp());
				outputFile.writeCharacter(' ');
				outputFile.writeInt(heroes[i].getHp());
				outputFile.writeCharacter(' ');
				outputFile.writeInt(heroes[i].getRow());
				outputFile.writeCharacter(' ');
				outputFile.writeInt(heroes[i].getCol());
				outputFile.writeCharacter('\n');
			}
			
			else
				outputFile.writeWord("dead\n");
		}
	}
}

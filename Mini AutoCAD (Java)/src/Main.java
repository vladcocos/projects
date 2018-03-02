import java.io.File;
import java.io.IOException;
import java.util.Scanner;
import javax.imageio.ImageIO;
import image.Image;
import tools.Factory;
import tools.Sketcher;

public final class Main {
    private Main() {
    }

    public static void main(final String[] args) throws IOException {
        // Initializare fisiere input si output, scanner pentru a citi din
        // fisierul de input
        File inputFile = new File(args[0]);
        File outputFile = new File("drawing.png");
        Scanner scan = new Scanner(inputFile);

        // Initializare imagine, factory si sketcher
        Image image = new Image();
        Factory shapeFactory = Factory.getFactory();
        Sketcher sketcher = new Sketcher();

        // Citire numar de forme din fisierul de input
        int numberOfShapes = Integer.parseInt(scan.nextLine());

        // Citirea fiecarei forme din fisierul de input si desenarea acestuia in
        // imagine
        for (int i = 0; i < numberOfShapes; i++) {
            shapeFactory.getShape(scan.nextLine()).getDrawn(sketcher, image);
        }

        ImageIO.write(image.getImage(), "png", outputFile);
        scan.close();
    }
}

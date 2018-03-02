package tools;

import java.awt.image.BufferedImage;

import image.Image;
import shapes.Pixel;
import shapes.Canvas;
import shapes.Circle;
import shapes.Diamond;
import shapes.Line;
import shapes.Polygon;
import shapes.Rectangle;
import shapes.Square;
import shapes.Triangle;

public class Sketcher implements DrawTool {
    private DrawAlgorithm sketcher = new DrawAlgorithm();

    // Metoda pentru desenare Canvas: initializeaza BufferedImage din clasa
    // Image cu inaltimea si latimea Canvas-ului
    @Override
    public final void draw(final Canvas canvas, final Image image) {
        image.setImage(new BufferedImage(canvas.getWidth(), canvas.getHeight(),
                BufferedImage.TYPE_INT_ARGB));

        // Coloreaza toti pixelii din cadrul canvasului cu culoarea acestuia
        for (int i = 0; i < canvas.getWidth(); i++) {
            for (int j = 0; j < canvas.getHeight(); j++) {
                image.getImage().setRGB(i, j,
                        (canvas.getFillOpacity() << ALPHA_SHIFT) + canvas.getFillColor());
            }
        }
    }

    // Metoda pentru desenarea unei linii folosind metoda drawLine din clasa
    // DrawAlgorithm
    @Override
    public final void draw(final Line line, final Image image) {
        sketcher.drawLine(line.getStartX(), line.getStartY(), line.getFinalX(), line.getFinalY(),
                image, (line.getFillOpacity() << ALPHA_SHIFT) + line.getFillColor());
    }

    // Metoda pentru desenarea unui patrat trasand, pe rand, toate cele 4 laturi
    // si colorarea interiorului acestuia
    @Override
    public final void draw(final Square square, final Image image) {
        // Trasarea celor 4 laturi
        sketcher.drawLine(square.getStartX(), square.getStartY(),
                square.getStartX() + square.getSide() - 1, square.getStartY(), image,
                (square.getStrokeOpacity() << ALPHA_SHIFT) + square.getStrokeColor());

        sketcher.drawLine(square.getStartX() + square.getSide() - 1, square.getStartY(),
                square.getStartX() + square.getSide() - 1,
                square.getStartY() + square.getSide() - 1, image,
                (square.getStrokeOpacity() << ALPHA_SHIFT) + square.getStrokeColor());

        sketcher.drawLine(square.getStartX() + square.getSide() - 1,
                square.getStartY() + square.getSide() - 1, square.getStartX(),
                square.getStartY() + square.getSide() - 1, image,
                (square.getStrokeOpacity() << ALPHA_SHIFT) + square.getStrokeColor());

        sketcher.drawLine(square.getStartX(), square.getStartY() + square.getSide() - 1,
                square.getStartX(), square.getStartY(), image,
                (square.getStrokeOpacity() << ALPHA_SHIFT) + square.getStrokeColor());

        // Coloreaza interiorul patratului
        for (int i = square.getStartX() + 1; i < square.getStartX() + square.getSide() - 1; i++) {
            for (int j = square.getStartY() + 1; j < square.getStartY() + square.getSide()
                    - 1; j++) {
                if (i < image.getImage().getWidth() && j < image.getImage().getHeight()) {
                    image.getImage().setRGB(i, j,
                            (square.getFillOpacity() << ALPHA_SHIFT) + square.getFillColor());
                }
            }
        }
    }

    // Metoda pentru desenarea unui dreptunghi trasand, pe rand, toate cele 4
    // laturi si colorarea interiorului acestuia
    @Override
    public final void draw(final Rectangle rectangle, final Image image) {
        // Trasarea celor 4 laturi
        sketcher.drawLine(rectangle.getStartX(), rectangle.getStartY(),
                rectangle.getStartX() + rectangle.getWidth() - 1, rectangle.getStartY(), image,
                (rectangle.getStrokeOpacity() << ALPHA_SHIFT) + rectangle.getStrokeColor());

        sketcher.drawLine(rectangle.getStartX() + rectangle.getWidth() - 1, rectangle.getStartY(),
                rectangle.getStartX() + rectangle.getWidth() - 1,
                rectangle.getStartY() + rectangle.getHeight() - 1, image,
                (rectangle.getStrokeOpacity() << ALPHA_SHIFT) + rectangle.getStrokeColor());

        sketcher.drawLine(rectangle.getStartX() + rectangle.getWidth() - 1,
                rectangle.getStartY() + rectangle.getHeight() - 1, rectangle.getStartX(),
                rectangle.getStartY() + rectangle.getHeight() - 1, image,
                (rectangle.getStrokeOpacity() << ALPHA_SHIFT) + rectangle.getStrokeColor());

        sketcher.drawLine(rectangle.getStartX(), rectangle.getStartY() + rectangle.getHeight() - 1,
                rectangle.getStartX(), rectangle.getStartY(), image,
                (rectangle.getStrokeOpacity() << ALPHA_SHIFT) + rectangle.getStrokeColor());

        // Coloreaza interiorul dreptunghiului
        for (int i = rectangle.getStartX() + 1; i < rectangle.getStartX() + rectangle.getWidth()
                - 1; i++) {
            for (int j = rectangle.getStartY() + 1; j < rectangle.getStartY()
                    + rectangle.getHeight() - 1; j++) {
                if (i < image.getImage().getWidth() && j < image.getImage().getHeight()) {
                    image.getImage().setRGB(i, j,
                            (rectangle.getFillOpacity() << ALPHA_SHIFT) + rectangle.getFillColor());
                }
            }
        }
    }

    // Metoda pentru desenarea unui cerc folosind metodele drawCircle (pentru
    // trasarea conturului) si fill (pentru colorarea interiorului) din clasa
    // DrawAlgorithm
    @Override
    public final void draw(final Circle circle, final Image image) {
        // Trasarea conturului
        sketcher.drawCircle(circle.getStartX(), circle.getStartY(), circle.getRadius(), image,
                (circle.getStrokeOpacity() << ALPHA_SHIFT) + circle.getStrokeColor());

        Pixel startPixel = new Pixel(circle.getStartX(), circle.getStartY());

        // Colorarea interiorului cercului
        sketcher.fill(startPixel, (circle.getFillOpacity() << ALPHA_SHIFT) + circle.getFillColor(),
                (circle.getStrokeOpacity() << ALPHA_SHIFT) + circle.getStrokeColor(), image);
    }

    // Metoda pentru desenarea unui triunghi trasand, pe rand, toate cele 3
    // laturi si colorarea acestuia folosind metoda fill din clasa DrawAlgorithm
    @Override
    public final void draw(final Triangle triangle, final Image image) {
        // Trasarea celor 3 laturi
        sketcher.drawLine(triangle.getFirstPointX(), triangle.getFirstPointY(),
                triangle.getSecondPointX(), triangle.getSecondPointY(), image,
                (triangle.getStrokeOpacity() << ALPHA_SHIFT) + triangle.getStrokeColor());

        sketcher.drawLine(triangle.getSecondPointX(), triangle.getSecondPointY(),
                triangle.getThirdPointX(), triangle.getThirdPointY(), image,
                (triangle.getStrokeOpacity() << ALPHA_SHIFT) + triangle.getStrokeColor());

        sketcher.drawLine(triangle.getThirdPointX(), triangle.getThirdPointY(),
                triangle.getFirstPointX(), triangle.getFirstPointY(), image,
                (triangle.getStrokeOpacity() << ALPHA_SHIFT) + triangle.getStrokeColor());

        // Calcularea coordonatelor centrului de greutate
        Pixel startPixel = new Pixel(
                (triangle.getFirstPointX() + triangle.getSecondPointX() + triangle.getThirdPointX())
                        / TRIANGLE_POINTS,
                (triangle.getFirstPointY() + triangle.getSecondPointY() + triangle.getThirdPointY())
                        / TRIANGLE_POINTS);

        // Colorarea interiorului triunghiului
        sketcher.fill(startPixel,
                (triangle.getFillOpacity() << ALPHA_SHIFT) + triangle.getFillColor(),
                (triangle.getStrokeOpacity() << ALPHA_SHIFT) + triangle.getStrokeColor(), image);
    }

    // Metoda pentru desenarea unui romb trasand, pe rand, cele 4 laturi si
    // colorarea acestuia folosind metoda fill din clasa DrawAlgorithm
    @Override
    public final void draw(final Diamond diamond, final Image image) {
        // Trasarea celor 4 laturi
        sketcher.drawLine(diamond.getStartX(), diamond.getStartY() - diamond.getWidth() / 2,
                diamond.getStartX() - diamond.getHeight() / 2, diamond.getStartY(), image,
                (diamond.getStrokeOpacity() << ALPHA_SHIFT) + diamond.getStrokeColor());

        sketcher.drawLine(diamond.getStartX() - diamond.getHeight() / 2, diamond.getStartY(),
                diamond.getStartX(), diamond.getStartY() + diamond.getWidth() / 2, image,
                (diamond.getStrokeOpacity() << ALPHA_SHIFT) + diamond.getStrokeColor());

        sketcher.drawLine(diamond.getStartX(), diamond.getStartY() + diamond.getWidth() / 2,
                diamond.getStartX() + diamond.getHeight() / 2, diamond.getStartY(), image,
                (diamond.getStrokeOpacity() << ALPHA_SHIFT) + diamond.getStrokeColor());

        sketcher.drawLine(diamond.getStartX() + diamond.getHeight() / 2, diamond.getStartY(),
                diamond.getStartX(), diamond.getStartY() - diamond.getWidth() / 2, image,
                (diamond.getStrokeOpacity() << ALPHA_SHIFT) + diamond.getStrokeColor());

        Pixel startPixel = new Pixel(diamond.getStartX(), diamond.getStartY());

        // Colorarea interiorului rombului
        sketcher.fill(startPixel,
                (diamond.getFillOpacity() << ALPHA_SHIFT) + diamond.getFillColor(),
                (diamond.getStrokeOpacity() << ALPHA_SHIFT) + diamond.getStrokeColor(), image);
    }

    // Metoda pentru desenarea unui poligon trasand, pe rand, toate laturile
    // sale si colorarea acestuia folosind metoda fill din clasa DrawAlgorithm
    @Override
    public final void draw(final Polygon polygon, final Image image) {
        // Trasarea celor n - 1 laturi (poligon cu n laturi)
        for (int i = 0; i < polygon.getNumberOfPoints() - 1; i++) {
            sketcher.drawLine(polygon.getXPoints()[i], polygon.getYPoints()[i],
                    polygon.getXPoints()[i + 1], polygon.getYPoints()[i + 1], image,
                    (polygon.getStrokeOpacity() << ALPHA_SHIFT) + polygon.getStrokeColor());
        }

        // Trasarea ultimii laturi intre ultimul punct si primul
        sketcher.drawLine(polygon.getXPoints()[polygon.getNumberOfPoints() - 1],
                polygon.getYPoints()[polygon.getNumberOfPoints() - 1], polygon.getXPoints()[0],
                polygon.getYPoints()[0], image,
                (polygon.getStrokeOpacity() << ALPHA_SHIFT) + polygon.getStrokeColor());

        // Calcularea coordonatelor centrului de greutate
        int startX = 0;
        int startY = 0;
        for (int i = 0; i < polygon.getNumberOfPoints(); i++) {
            startX += polygon.getXPoints()[i];
            startY += polygon.getYPoints()[i];
        }

        startX = startX / polygon.getNumberOfPoints();
        startY = startY / polygon.getNumberOfPoints();

        Pixel startPixel = new Pixel(startX, startY);

        // Colorarea interiorului poligonului
        sketcher.fill(startPixel,
                (polygon.getFillOpacity() << ALPHA_SHIFT) + polygon.getFillColor(),
                (polygon.getStrokeOpacity() << ALPHA_SHIFT) + polygon.getStrokeColor(), image);
    }

}

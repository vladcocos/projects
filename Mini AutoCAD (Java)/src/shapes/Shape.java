package shapes;

import image.Image;
import tools.DrawTool;
import tools.DrawableShape;

public abstract class Shape implements DrawableShape {
    private int strokeColor;
    private int strokeOpacity;
    private int fillColor;
    private int fillOpacity;

    public final int getStrokeColor() {
        return strokeColor;
    }

    public final void setStrokeColor(final int strokeColor) {
        this.strokeColor = strokeColor;
    }

    public final int getStrokeOpacity() {
        return strokeOpacity;
    }

    public final void setStrokeOpacity(final int strokeOpacity) {
        this.strokeOpacity = strokeOpacity;
    }

    public final int getFillColor() {
        return fillColor;
    }

    public final void setFillColor(final int fillColor) {
        this.fillColor = fillColor;
    }

    public final int getFillOpacity() {
        return fillOpacity;
    }

    public final void setFillOpacity(final int fillOpacity) {
        this.fillOpacity = fillOpacity;
    }

    @Override
    public abstract void getDrawn(DrawTool drawTool, Image image);
}

package image;

import java.awt.image.BufferedImage;

public class Image {
    private BufferedImage image;

    public final BufferedImage getImage() {
        return image;
    }

    public final void setImage(final BufferedImage image) {
        this.image = image;
    }
}

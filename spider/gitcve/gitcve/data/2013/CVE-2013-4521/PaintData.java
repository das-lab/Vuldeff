package org.richfaces.demo.paint2d;

import java.io.Serializable;

public class PaintData implements Serializable{
	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	String text;
	Integer color;
	float scale;


	public Integer getColor() {
		return color;
	}
	public void setColor(Integer color) {
		this.color = color;
	}
	public float getScale() {
		return scale;
	}
	public void setScale(float scale) {
		this.scale = scale;
	}

	public String getText() {
		return text;
	}
	public void setText(String text) {
		this.text = text;
	}
}

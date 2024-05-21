/**
 * License Agreement.
 *
 *  JBoss RichFaces - Ajax4jsf Component Library
 *
 * Copyright (C) 2007  Exadel, Inc.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License version 2.1 as published by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
 */

package org.richfaces.renderkit.html;


import java.awt.Color;
import java.awt.Dimension;
import java.awt.Graphics2D;
import java.awt.image.BufferedImage;
import java.io.IOException;
import java.io.Serializable;

import javax.faces.FacesException;
import javax.faces.component.UIComponentBase;
import javax.faces.context.FacesContext;
import javax.faces.el.MethodBinding;

import org.ajax4jsf.resource.GifRenderer;
import org.ajax4jsf.resource.ImageRenderer;
import org.ajax4jsf.resource.InternetResourceBase;
import org.ajax4jsf.resource.JpegRenderer;
import org.ajax4jsf.resource.PngRenderer;
import org.ajax4jsf.resource.ResourceContext;
import org.ajax4jsf.resource.ResourceRenderer;
import org.ajax4jsf.util.HtmlColor;
import org.richfaces.component.UIPaint2D;

/**
 * Resource for create image by managed bean method
 * @author asmirnov@exadel.com (latest modification by $Author: aizobov $)
 * @version $Revision: 1.4 $ $Date: 2007/02/28 10:35:23 $
 *
 */
public class Paint2DResource extends InternetResourceBase {
	
	private static final ImageRenderer[] _renderers= {new GifRenderer(), new JpegRenderer(), new PngRenderer()};

//	private static final ThreadLocal<String> threadLocalContentType = new ThreadLocal<String>();

	
	/* (non-Javadoc)
	 * @see org.ajax4jsf.resource.InternetResourceBase#getRenderer()
	 */
	public ResourceRenderer getRenderer() {
		return _renderers[0];
	}

	public ResourceRenderer getRenderer(ResourceContext context) {
		ImageData data = null;
		
		if (context != null) {
			data = (ImageData) restoreData(context);
		}
		
		ImageRenderer renderer = _renderers[null==data?0:data._format];
		return renderer;
	}
	/* (non-Javadoc)
	 * @see org.ajax4jsf.resource.InternetResourceBase#isCacheable()
	 */
	public boolean isCacheable() {
		return false;
	}
	
	public boolean isCacheable(ResourceContext resourceContext) {
		ImageData data = (ImageData) restoreData(resourceContext);
		return data.cacheable;
	}

	/* (non-Javadoc)
	 * @see org.ajax4jsf.resource.InternetResourceBase#requireFacesContext()
	 */
	public boolean requireFacesContext() {
		// work in context
		return true;
	}

	
	/* (non-Javadoc)
	 * @see org.ajax4jsf.resource.InternetResourceBase#getDataToStore(javax.faces.context.FacesContext, java.lang.Object)
	 */
	protected Object getDataToStore(FacesContext context, Object data) {
		if (data instanceof UIPaint2D) {
			UIPaint2D paint2D = (UIPaint2D) data;
			ImageData dataToStore = new ImageData();
			dataToStore._width = paint2D.getWidth();
			dataToStore._height = paint2D.getHeight();
			dataToStore._data = paint2D.getData();
			
			dataToStore._paint = UIComponentBase.saveAttachedState(context, paint2D.getPaint());
			String format = paint2D.getFormat();
			if("jpeg".equalsIgnoreCase(format)) {
				dataToStore._format = 1;
			} else if("png".equalsIgnoreCase(format)) {
				dataToStore._format = 2;
			}
			
			String bgColor = paint2D.getBgcolor();
			try {
				dataToStore._bgColor = HtmlColor.decode(bgColor).getRGB();
			} catch (Exception e) {}
			dataToStore.cacheable = paint2D.isCacheable();
			return dataToStore;
			
		} else {
			throw new FacesException("Data for painting image resource not instance of UIPaint2D");
		}
	}

	private static final class ImageData implements Serializable {

		private static final long serialVersionUID = 4452040100045367728L;
		
		int _width=1;
		int _height = 1;
		Object _data;
		int _format = 0;
		Object _paint;
		boolean cacheable = false;
		/*
		 * init color with transparent by default
		 */
		int _bgColor = 0;
	}
	/**
	 * Primary calculation of image dimensions - used when HTML code is generated
	 * to render IMG's width and height
	 * Subclasses should override this method to provide correct sizes of rendered images
	 * @param facesContext
	 * @return dimensions of the image to be displayed on page
	 */
	public Dimension getDimensions(FacesContext facesContext, Object data){
		if (data instanceof UIPaint2D) {
			UIPaint2D paint2D = (UIPaint2D) data;
			return new Dimension(paint2D.getWidth(),paint2D.getHeight());
		}
		return new Dimension(1,1);
	}
	/**
	 * Secondary calculation is used basically by  getImage method
	 * @param resourceContext
	 * @return
	 */
	protected Dimension getDimensions(ResourceContext resourceContext){
		ImageData data = (ImageData) restoreData(resourceContext);
		return new Dimension(data._width,data._height);
	}
	
	/* (non-Javadoc)
	 * @see org.ajax4jsf.resource.InternetResourceBase#send(javax.faces.context.FacesContext, java.lang.Object)
	 */
	public void send(ResourceContext context) throws IOException {
		ImageData data = (ImageData) restoreData(context);
		ImageRenderer renderer = (ImageRenderer) getRenderer(context);
		FacesContext facesContext = FacesContext.getCurrentInstance();
		try {
			BufferedImage image = renderer.createImage(data._width,data._height);
			Graphics2D graphics = image.createGraphics();
			
			try {
				if (data._bgColor != 0) {
					Color color = new Color(data._bgColor);
					graphics.setBackground(color);
					graphics.clearRect(0, 0, data._width, data._height);
				}
				
				MethodBinding paint = (MethodBinding) UIComponentBase.restoreAttachedState(facesContext, data._paint);
				paint.invoke(facesContext, new Object[] {graphics,data._data});
			} finally {
				if (graphics != null) {
					graphics.dispose();
				}
			}
			
			renderer.sendImage(context, image);
		} catch (Exception e) {
//			log.error("Error send image from resource "+context.getPathInfo(),e);
			throw new FacesException("Error send image ",e);
		}
	}

//	public String getContentType(ResourceContext context) {
//		Object contentType = threadLocalContentType.get();
//		if (contentType != null) {
//			return (String) contentType;
//		} else {
//			return super.getContentType(context);
//		}
//	}
	
	/* (non-Javadoc)
	 * @see org.ajax4jsf.resource.InternetResourceBase#sendHeaders(org.ajax4jsf.resource.ResourceContext)
	 */
//	public void sendHeaders(ResourceContext context) {
//		ImageData data = (ImageData) restoreData(context);
//		ImageRenderer renderer = _renderers[data._format];
//		threadLocalContentType.set(renderer.getContentType());
//
//		super.sendHeaders(context);
//		
//		threadLocalContentType.set(null);
//	}
}

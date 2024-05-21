/**
 * License Agreement.
 *
 * Rich Faces - Natural Ajax for Java Server Faces (JSF)
 *
 * Copyright (C) 2007 Exadel, Inc.
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

package org.ajax4jsf.resource;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.io.StreamCorruptedException;
import java.io.StringReader;
import java.io.UnsupportedEncodingException;
import java.net.URL;
import java.util.Collections;
import java.util.Date;
import java.util.Enumeration;
import java.util.HashMap;
import java.util.Map;
import java.util.regex.Matcher;
import java.util.regex.Pattern;
import java.util.zip.Deflater;
import java.util.zip.Inflater;

import javax.faces.FacesException;
import javax.faces.context.FacesContext;
import javax.imageio.ImageIO;
import javax.servlet.ServletContext;

import org.ajax4jsf.Messages;
import org.ajax4jsf.resource.util.URLToStreamHelper;
import org.ajax4jsf.util.base64.Codec;
import org.ajax4jsf.webapp.WebXml;
import org.apache.commons.digester.Digester;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.xml.sax.EntityResolver;
import org.xml.sax.InputSource;
import org.xml.sax.SAXException;

/**
 * Produce instances of InternetResource's for any types - jar resource, dynamic
 * created image, component-incapsulated etc. Realised as singleton class to
 * support cache, configuration etc.
 * 
 * @author shura (latest modification by $Author: alexsmirnov $)
 * @version $Revision: 1.1.2.1 $ $Date: 2007/01/09 18:56:58 $
 * 
 */
public class ResourceBuilderImpl extends InternetResourceBuilder {

	private static final Log log = LogFactory.getLog(ResourceBuilderImpl.class);

	private static final String DATA_SEPARATOR = "/DATA/";
	private static final String DATA_BYTES_SEPARATOR = "/DATB/";

	private static final Pattern DATA_SEPARATOR_PATTERN = Pattern
			.compile("/DAT(A|B)/");


	private static final ResourceRenderer defaultRenderer = new MimeRenderer(null);

	private Map<String, ResourceRenderer> renderers;
	/**
	 * keep resources instances . TODO - put this map to application-scope
	 * attribute, for support clastering environment.
	 */
	private Map<String, InternetResource> resources = Collections.synchronizedMap(new HashMap<String, InternetResource>());

	private long _startTime;

	private Codec codec;

	private static final ResourceRenderer scriptRenderer = new ScriptRenderer();
	
	private static final ResourceRenderer styleRenderer = new StyleRenderer();

	static {
		// renderers.put(".htc",new BehaviorRenderer());
		// set in-memory caching ImageIO
		Thread thread = Thread.currentThread();
		ClassLoader initialTCCL = thread.getContextClassLoader();
		
		try {
			ClassLoader systemCL = ClassLoader.getSystemClassLoader();
			thread.setContextClassLoader(systemCL);
			ImageIO.setUseCache(false);
		} finally {
			thread.setContextClassLoader(initialTCCL);
		}

	}

	public WebXml getWebXml(FacesContext context) {
		WebXml webXml = WebXml.getInstance(context);
		if (null == webXml) {
			throw new FacesException(
					"Resources framework is not initialised, check web.xml for Filter configuration");
		}
		return webXml;
	}

	public ResourceBuilderImpl() {
		super();
		_startTime = System.currentTimeMillis();
		codec = new Codec();
		renderers = new HashMap<String, ResourceRenderer>();
		// append known renderers for extentions.
		renderers.put(".gif", new GifRenderer());
		ResourceRenderer renderer = new JpegRenderer();
		renderers.put(".jpeg", renderer);
		renderers.put(".jpg", renderer);
		renderers.put(".png", new PngRenderer());
		renderers.put(".js", getScriptRenderer());
		renderers.put(".css", getStyleRenderer());
		renderers.put(".log", new LogfileRenderer());
		renderers.put(".html", new HTMLRenderer());
		renderers.put(".xhtml", new MimeRenderer("application/xhtml+xml"));
		renderers.put(".xml", new MimeRenderer("text/xml"));
		renderers.put(".xcss", new TemplateCSSRenderer());
		renderers.put(".swf", new MimeRenderer("application/x-shockwave-flash"));
	}

	/**
	 * @throws FacesException
	 */
	protected void registerResources() throws FacesException {
		try {
			ClassLoader loader = Thread.currentThread().getContextClassLoader();
			Enumeration<URL> e = loader
					.getResources("META-INF/resources-config.xml");
			while (e.hasMoreElements()) {
				URL resource = e.nextElement();
				registerConfig(resource);
			}
		} catch (IOException e) {
			throw new FacesException(e);
		}
	}

	private void registerConfig(URL resourceConfig) {
		try {
			if (log.isDebugEnabled()) {
				log.debug("Process resources configuration file "
						+ resourceConfig.toExternalForm());
			}

			InputStream in = URLToStreamHelper.urlToStream(resourceConfig);
			try {
				Digester digester = new Digester();
				digester.setValidating(false);
				digester.setEntityResolver(new EntityResolver() {
					// Dummi resolver - alvays do nothing
					public InputSource resolveEntity(String publicId,
							String systemId) throws SAXException, IOException {
						return new InputSource(new StringReader(""));
					}
				});
				digester.setNamespaceAware(false);
				digester.setUseContextClassLoader(true);
				digester.push(this);
				digester.addObjectCreate("resource-config/resource", "class",
						JarResource.class);
				digester.addObjectCreate("resource-config/resource/renderer",
						"class", HTMLRenderer.class);
				digester.addCallMethod(
						"resource-config/resource/renderer/content-type",
						"setContentType", 0);
				digester.addSetNext("resource-config/resource/renderer",
						"setRenderer", ResourceRenderer.class.getName());
				digester.addCallMethod("resource-config/resource/name",
						"setKey", 0);
				digester.addCallMethod("resource-config/resource/path",
						"setPath", 0);
				digester.addCallMethod("resource-config/resource/cacheable",
						"setCacheable", 0);
				digester.addCallMethod(
						"resource-config/resource/session-aware",
						"setSessionAware", 0);
				digester.addCallMethod("resource-config/resource/property",
						"setProperty", 2);
				digester.addCallParam("resource-config/resource/property/name",
						0);
				digester.addCallParam(
						"resource-config/resource/property/value", 1);
				digester.addCallMethod("resource-config/resource/content-type",
						"setContentType", 0);
				digester.addSetNext("resource-config/resource", "addResource",
						InternetResource.class.getName());
				digester.parse(in);
			} finally {
				in.close();
			}
		} catch (IOException e) {
			throw new FacesException(e);
		} catch (SAXException e) {
			throw new FacesException(e);
		}
	}

	/**
	 */
	public void init() throws FacesException {
		// TODO - mace codec configurable.
		registerResources();
	}

	/**
	 * Base point for creating resource. Must detect type and build appropriate
	 * instance. Currently - make static resource for ordinary request, or
	 * instance of class.
	 * 
	 * @param base
	 *            base object for resource ( resourcess in classpath will be get
	 *            relative to it package )
	 * @param path
	 *            key - path to resource, resource class name etc.
	 * @return
	 * @throws FacesException
	 */
	public InternetResource createResource(Object base, String path)
			throws FacesException {
		// TODO - detect type of resource ( for example, resources location path
		// in Skin
		try {
			return getResource(path);
		} catch (ResourceNotFoundException e) {
			try {
				return getResource(buildKey(base, path));
			} catch (ResourceNotFoundException e1) {
				if (log.isDebugEnabled()) {
					log.debug(Messages.getMessage(Messages.BUILD_RESOURCE_INFO,
							path));
				}
			}
		}
		// path - is class name ?
		InternetResource res;
		try {
			Class<?> resourceClass = Class.forName(path);
			res = createDynamicResource(path, resourceClass);
		} catch (Exception e) {
			try {
				res = createJarResource(base, path);
			} catch (ResourceNotFoundException ex) {
				res = createStaticResource(path);
			}
			// TODO - if resource not found, create static ?
		}
		return res;
	}

	private String buildKey(Object base, String path) {
		if (path.startsWith("/")) {
			return path.substring(1);
		}
		if (null == base) {
			return path;
		}
		StringBuffer packageName = new StringBuffer(base.getClass()
				.getPackage().getName().replace('.', '/'));
		return packageName.append("/").append(path).toString();
	}

	public String getUri(InternetResource resource, FacesContext context,
			Object storeData) {
		StringBuffer uri = new StringBuffer();// ResourceServlet.DEFAULT_SERVLET_PATH).append("/");
		uri.append(resource.getKey());
		// append serialized data as Base-64 encoded request string.
		if (storeData != null) {
			try {
				byte[] objectData;
				if (storeData instanceof byte[]) {
					objectData = (byte[]) storeData;
					uri.append(DATA_BYTES_SEPARATOR);
				} else {
					ByteArrayOutputStream dataSteram = new ByteArrayOutputStream(
							1024);
					ObjectOutputStream objStream = new ObjectOutputStream(
							dataSteram);
					objStream.writeObject(storeData);
					objStream.flush();
					objStream.close();
					dataSteram.close();
					objectData = dataSteram.toByteArray();
					uri.append(DATA_SEPARATOR);
				}
				byte[] dataArray = encrypt(objectData);
				uri.append(new String(dataArray, "ISO-8859-1"));

				// / byte[] objectData = dataSteram.toByteArray();
				// / uri.append("?").append(new
				// String(Base64.encodeBase64(objectData),
				// / "ISO-8859-1"));
			} catch (Exception e) {
				// Ignore errors, log it
				log.error(Messages
						.getMessage(Messages.QUERY_STRING_BUILDING_ERROR), e);
			}
		}
		
		boolean isGlobal = !resource.isSessionAware();
		
		String resourceURL = getWebXml(context).getFacesResourceURL(context,
				uri.toString(), isGlobal);// context.getApplication().getViewHandler().getResourceURL(context,uri.toString());
		if (!isGlobal) {
			resourceURL = context.getExternalContext().encodeResourceURL(
					resourceURL);
		}
		if (log.isDebugEnabled()) {
			log.debug(Messages.getMessage(Messages.BUILD_RESOURCE_URI_INFO,
					resource.getKey(), resourceURL));
		}
		return resourceURL;// context.getExternalContext().encodeResourceURL(resourceURL);

	}

	/**
	 * @param key
	 * @return
	 */
	public InternetResource getResourceForKey(String key)
			throws ResourceNotFoundException {

		Matcher matcher = DATA_SEPARATOR_PATTERN.matcher(key);
		if (matcher.find()) {
			int data = matcher.start();
			key = key.substring(0, data);
		}

		return getResource(key);
	}

	public Object getResourceDataForKey(String key) {
		Object data = null;
		String dataString = null;
		Matcher matcher = DATA_SEPARATOR_PATTERN.matcher(key);
		if (matcher.find()) {
			if (log.isDebugEnabled()) {
				log.debug(Messages.getMessage(
						Messages.RESTORE_DATA_FROM_RESOURCE_URI_INFO, key,
						dataString));
			}
			int dataStart = matcher.end();
			dataString = key.substring(dataStart);
			byte[] objectArray = null;
			byte[] dataArray;
			try {
				dataArray = dataString.getBytes("ISO-8859-1");
				objectArray = decrypt(dataArray);
			} catch (UnsupportedEncodingException e1) {
				// default encoding always presented.
			}
			if ("B".equals(matcher.group(1))) {
				data = objectArray;
			} else {
				try {
					ObjectInputStream in = new ObjectInputStream(
							new ByteArrayInputStream(objectArray));
					data = in.readObject();
				} catch (StreamCorruptedException e) {
					log.error(Messages
							.getMessage(Messages.STREAM_CORRUPTED_ERROR), e);
				} catch (IOException e) {
					log.error(Messages
							.getMessage(Messages.DESERIALIZE_DATA_INPUT_ERROR),
							e);
				} catch (ClassNotFoundException e) {
					log
							.error(
									Messages
											.getMessage(Messages.DATA_CLASS_NOT_FOUND_ERROR),
									e);
				}
			}
		}

		return data;
	}

	public InternetResource getResource(String path)
			throws ResourceNotFoundException {

		InternetResource internetResource = (InternetResource) resources
				.get(path);
		if (null == internetResource) {
			throw new ResourceNotFoundException("Resource not registered : "
					+ path);
		} else {
			return internetResource;
		}
	}

	public void addResource(InternetResource resource) {
		resources.put(resource.getKey(), resource);
		ResourceRenderer renderer = resource.getRenderer(null);
		if (renderer == null) {
			setRenderer(resource, resource.getKey());
		}
	}

	public void addResource(String key, InternetResource resource) {
		resources.put(key, resource);
		resource.setKey(key);
		// TODO - set renderer ?
	}

	// public String getFacesResourceKey(HttpServletRequest request) {
	// return getWebXml(context).getFacesResourceKey(request);
	// }

	public String getFacesResourceURL(FacesContext context, String Url, boolean isGlobal) {
		return getWebXml(context).getFacesResourceURL(context, Url, isGlobal);
	}

	/**
	 * Build resource for link to static context in webapp.
	 * 
	 * @param path
	 * @return
	 * @throws FacesException
	 */
	protected InternetResource createStaticResource(String path)
			throws ResourceNotFoundException, FacesException {
		FacesContext context = FacesContext.getCurrentInstance();
		if (null != context) {
			if (context.getExternalContext().getContext() instanceof ServletContext) {
				ServletContext servletContext = (ServletContext) context
						.getExternalContext().getContext();
				InputStream in = servletContext.getResourceAsStream(path);
				if (null != in) {
					InternetResourceBase res = new StaticResource(path);
					setRenderer(res, path);
					res.setLastModified(new Date(getStartTime()));
					addResource(path, res);
					try {
						in.close();
					} catch (IOException e) {
					}
					return res;
				}
			}
		}
		throw new ResourceNotFoundException(Messages.getMessage(
				Messages.STATIC_RESOURCE_NOT_FOUND_ERROR, path));
	}

	private void setRenderer(InternetResource res, String path)
			throws FacesException {
		int lastPoint = path.lastIndexOf('.');
		if (lastPoint > 0) {
			String ext = path.substring(lastPoint);
			ResourceRenderer resourceRenderer =  getRendererByExtension(ext);
			if (null != resourceRenderer) {
				res.setRenderer(resourceRenderer);
			} else {
				if (log.isDebugEnabled()) {
					log.debug(Messages.getMessage(
							Messages.NO_RESOURCE_REGISTERED_ERROR_2, path,
							renderers.keySet()));
				}

				// String mimeType = servletContext.getMimeType(path);
				res.setRenderer(defaultRenderer);
			}
		}
	}

	/**
	 * @param ext
	 * @return
	 */
	protected ResourceRenderer getRendererByExtension(String ext) {
		return renderers
				.get(ext);
	}

	/**
	 * Create resurce to send from classpath relative to base class.
	 * 
	 * @param base
	 * @param path
	 * @return
	 * @throws FacesException
	 */
	protected InternetResource createJarResource(Object base, String path)
			throws ResourceNotFoundException, FacesException {
		String key = buildKey(base, path);
		ClassLoader loader = Thread.currentThread().getContextClassLoader();
		if (null != loader.getResource(key)) {
			JarResource res = new JarResource(key);
			setRenderer(res, path);
			res.setLastModified(new Date(getStartTime()));
			addResource(key, res);
			return res;
		} else {
			throw new ResourceNotFoundException(Messages.getMessage(
					Messages.NO_RESOURCE_EXISTS_ERROR, key));
		}

	}

	/**
	 * Create resource by instatiate given class.
	 * 
	 * @param path
	 * @param instatiate
	 * @return
	 */
	protected InternetResource createDynamicResource(String path,
			Class<?> instatiate) throws ResourceNotFoundException {
		if (InternetResource.class.isAssignableFrom(instatiate)) {
			InternetResource resource;
			try {
				resource = (InternetResource) instatiate.newInstance();
				addResource(path, resource);
			} catch (Exception e) {
				String message = Messages.getMessage(
						Messages.INSTANTIATE_RESOURCE_ERROR, instatiate
								.toString());
				log.error(message, e);
				throw new ResourceNotFoundException(message, e);
			}
			return resource;
		}
		throw new FacesException(Messages
				.getMessage(Messages.INSTANTIATE_CLASS_ERROR));
	}

	/**
	 * Create resource by instatiate {@link UserResource} class with given
	 * properties ( or got from cache ).
	 * 
	 * @param cacheable
	 * @param session
	 * @param mime
	 * @return
	 * @throws FacesException
	 */
	public InternetResource createUserResource(boolean cacheable,
			boolean session, String mime) throws FacesException {
		String path = getUserResourceKey(cacheable, session, mime);
		InternetResource userResource;
		try {
			userResource = getResource(path);
		} catch (ResourceNotFoundException e) {
			userResource = new UserResource(cacheable, session, mime);
			addResource(path, userResource);
		}
		return userResource;
	}

	/**
	 * Generate resource key for user-generated resource with given properties.
	 * 
	 * @param cacheable
	 * @param session
	 * @param mime
	 * @return
	 */
	private String getUserResourceKey(boolean cacheable, boolean session,
			String mime) {
		StringBuffer pathBuffer = new StringBuffer(UserResource.class.getName());
		pathBuffer.append(cacheable ? "/c" : "/n");
		pathBuffer.append(session ? "/s" : "/n");
		if (null != mime) {
			pathBuffer.append('/').append(mime.hashCode());
		}
		String path = pathBuffer.toString();
		return path;
	}

	/**
	 * @return Returns the startTime for application.
	 */
	public long getStartTime() {
		return _startTime;
	}

	protected byte[] encrypt(byte[] src) {
		try {
			Deflater compressor = new Deflater(Deflater.BEST_SPEED);
			byte[] compressed = new byte[src.length + 100];
			compressor.setInput(src);
			compressor.finish();
			int totalOut = compressor.deflate(compressed);
			byte[] zipsrc = new byte[totalOut];
			System.arraycopy(compressed, 0, zipsrc, 0, totalOut);
			compressor.end();
			return codec.encode(zipsrc);
		} catch (Exception e) {
			throw new FacesException("Error encode resource data", e);
		}
	}

	protected byte[] decrypt(byte[] src) {
		try {
			byte[] zipsrc = codec.decode(src);
			Inflater decompressor = new Inflater();
			byte[] uncompressed = new byte[zipsrc.length * 5];
			decompressor.setInput(zipsrc);
			int totalOut = decompressor.inflate(uncompressed);
			byte[] out = new byte[totalOut];
			System.arraycopy(uncompressed, 0, out, 0, totalOut);
			decompressor.end();
			return out;
		} catch (Exception e) {
			throw new FacesException("Error decode resource data", e);
		}
	}

	@Override
	public ResourceRenderer getScriptRenderer() {
		return scriptRenderer;
	}

	@Override
	public ResourceRenderer getStyleRenderer() {
		return styleRenderer;
	}
}

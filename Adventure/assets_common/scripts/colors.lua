function Brown( brightness, alpha )
	if not alpha then
		alpha = 1.0
	end
	
	if not brightness then 
		brightness = 1.0
	end
	return RGBA( 107/255 * 2 * brightness, 88/255 * 2 * brightness, 37/255 * 2 * brightness, alpha )
end

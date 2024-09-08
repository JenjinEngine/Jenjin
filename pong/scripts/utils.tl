global function rgb_to_hsv(r: integer, g: integer, b: integer): integer, integer, integer
local h: integer
	local s: integer
	local v: integer
	local max: integer
	local min: integer
	local delta: integer
	max = math.max(r, g, b)
	min = math.min(r, g, b)
	v = max
	delta = max - min
	if max == 0 then
		s = 0
		else
		s = delta / max
	end
	if s == 0 then
		h = 0
		else
		if r == max then
			h = (g - b) / delta
			elseif g == max then
			h = 2 + (b - r) / delta
			else
			h = 4 + (r - g) / delta
		end
		h = h * 60
		if h < 0 then
			h = h + 360
		end
	end
	return h, s, v
end

global function hsv_to_rgb(h: integer, s: integer, v: integer): integer, integer, integer
local r: integer
	local g: integer
	local b: integer
	local i: integer
	local f: integer
	local p: integer
	local q: integer
	local t: integer
	if s == 0 then
		r = v
		g = v
		b = v
		else
		if h == 360 then
			h = 0
		end
		h = h / 60
		i = math.floor(h)
		f = h - i
		p = v * (1 - s)
		q = v * (1 - s * f)
		t = v * (1 - s * (1 - f))
		if i == 0 then
			r = v
			g = t
			b = p
			elseif i == 1 then
			r = q
			g = v
			b = p
			elseif i == 2 then
			r = p
			g = v
			b = t
			elseif i == 3 then
			r = p
			g = q
			b = v
			elseif i == 4 then
			r = t
			g = p
			b = v
			else
			r = v
			g = p
			b = q
		end
	end
	return r, g, b
end

return {
	rgb_to_hsv = rgb_to_hsv,
	hsv_to_rgb = hsv_to_rgb
}

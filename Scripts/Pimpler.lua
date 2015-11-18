filepath = arg[1]
print('filepath = ' .. filepath)
function FindFileName(path)
	local dotStart = string.find(path, '.cpp')	
	for i=dotStart, 1, -1 do
		if path:sub(i, i) == '\\' then
			return string.sub(filepath, i+1, dotStart - 1)
		end
	end
end
className = FindFileName(filepath)
print('className = ' .. className)
-- //#!PimplStart
-- //#!PimplEnd
function CaptureFunctionName(line)
	return string.match(line, "([%w_][%w%d_]+)%(")
end

function ReplaceToMethod(line, functionName)
	return string.gsub(line, functionName, string.format("%s::%s", className, functionName))
end

function CaptureParamList(line, functionName)
	local list = ''
	local params = string.match(line, '%b()')
	params = string.gsub(params, "%s*=%s*0", '')
	params = string.gsub(params, "%(", '')
	--print('params = '.. params)	
	
	local first = true
	for param in string.gmatch(params, "([%w_][%w%d_]*)[,%)]") do
		if first then
			list = list .. param
			first = false
		else
			list = list .. ', ' .. param
		end
	end
	return list
end

function NeedToReturn(line)
	for returnType in string.gmatch(line, "[%w_][%w%d_]*%**") do		
		if (returnType == 'void') then
			return false;
		end
		return true;
	end
end

if filepath then
	io.input(filepath)
	
	local lines ={}
	local started = false
	local successful = false
	for line in io.lines() do
		local specialLine= false
		if string.find(line, '//#!PimplStart') then
			started = true			
			specialLine = true
		elseif string.find(line, '//#!PimplEnd') then
			started = false			
			specialLine = true
		end
		if started then
			local commentStart = string.find(line, '//')
			if commentStart==nil or commentStart > 4   then
				local functionName = CaptureFunctionName(line)
				if functionName then
					line = ReplaceToMethod(line, functionName)
					--print('Funtion Name = ' .. functionName);
					local paramList = CaptureParamList(line, functionName)
					--print('param list = ' .. paramList)				
					--print('')
					local needToReturn = NeedToReturn(line)
					local new
					if needToReturn then
						new = string.gsub(line, ";", string.format(" {\n\treturn mImpl->%s(%s);\n}\n\n", functionName, paramList))
					else
						new = string.gsub(line, ";", string.format(" {\n\tmImpl->%s(%s);\n}\n\n", functionName, paramList))
					end
					lines[#lines+1] = new;
					successful = true
				end
			else
				if not specialLine then
					lines[#lines+1] = line .. '\n';
				end
			end
		else
			if not specialLine then
				lines[#lines+1] = line .. '\n';
			end
		end
	end
	io.input()
	io.output(filepath)
	if (successful) then
		for i=1, #lines do
			io.write(lines[i])
		end
	end
	io.output()
end
io.read()
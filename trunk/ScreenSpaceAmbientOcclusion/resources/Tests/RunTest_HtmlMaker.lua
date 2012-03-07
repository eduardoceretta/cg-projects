workingDir = "..\\.." -- from here

programPath = "prj/ssao/vc9/tssao_Debug" -- from working dir
programName = "tssao.exe"
logFilePath = "resources/Tests/logFileTmp.log"
htmlFileName = "resources/Tests/results.html"

screenReference = {
  "resources\\Tests\\ScreenShots\\reference\\DragonCam00.png",
  "resources\\Tests\\ScreenShots\\reference\\DragonCam01.png",
  "resources\\Tests\\ScreenShots\\reference\\DragonCam02.png",
  "resources\\Tests\\ScreenShots\\reference\\DragonCam03.png",
  "resources\\Tests\\ScreenShots\\reference\\DragonCam04.png",
  "resources\\Tests\\ScreenShots\\reference\\DragonCam05.png",
}

parameters = {
  {name = "-scenepath", values = {"resources/Scenes/scene.rt4"}}, --1
  {name = "-logFile", values = {logFilePath}}, --1
  {name = "-benchmark", values = {"True"}}, -- 1
  -- {name = "-Cone_angle", values = {"0.523598776", "0.523598776",  "0.785398163", "1.04719755"}}, --15,30,45,60  -- 4
  {name = "-Cone_angle", values = {"0.523598776", "0.523598776",  "1.04719755"}}, --15,30,45,60  -- 3
  -- {name = "-Cone_numCones", values = {"4", "5", "6", "7", "8", "9", "10", "15", "20", "30"}}, -- 10
  {name = "-Cone_numCones", values = {"4", "6", "30"}}, -- 5
  -- {name = "-Cone_numSpheres", values = {"1", "2", "3", "4", "5", "6", "7", "8", "9", "10"}}, -- 10
  {name = "-Cone_numSpheres", values = {"1", "3", "8"}}, -- 3
  -- {name = "-Cone_numSamplers", values = {"1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "15", "20", "30"}}, -- 13
  {name = "-Cone_numSamplers", values = {"2", "3", "8", }}, -- 3
  {name = "-Cone_infoMethod", values = {"InitDist", "Progression"}}, -- 2
  -- {name = "-Cone_infoSphereOverlap", values = {"0.0", ".1", ".2", ".3", ".4", ".5", ".6", ".7", ".8", ".9"}}, --10
  -- {name = "-Cone_infoSphereCenterParm", values = {".5", "1.0", "1.5", "2.0", "2.5", "3.0", "4.0", "5.0"}}, --9
  -- {name = "-Cone_infoSphereRadiusParm", values = {".5", "1.0", "1.5", "2.0", "2.5", "3.0", "4.0", "5.0"}}, --8
  -- {name = "-rfar ", values = {".1", ".2", ".3", ".4", ".5", ".6", ".7", ".8", ".9", "1.0"}}, --10 
  {name = "-rfar ", values = {".1", ".2", ".8"}}, -- 4
  -- {name = "-contrast ", values = {".5", ".75", "1.0", "1.25", "1.5", "2.0", "3.0", "5.0", "10.0", "100.0"}}, -- 10
  {name = "-contrast ", values = {".5", "1.0", "1.25"}}, -- 3
  {name = "-jitter", values = {"True", "False"}}, -- 2
}

-- 1*1*4*10*10*13*2*10*9*8*10*10*2 = 1497600000
-- 1*1*4*7*5*8*2*6*6*2 = 161280
-- 1*1*4*5*5*5*2*4*5*2 = 40000
-- 1*1*3*3*3*3*2*3*3*2 = 2916
-- 1*1*3*3*3*3*2*3*3*2 * 10/(60) = 48.6 min
args = {}

function RecurIterate(parmTable, resultTable, index, initStr)
  local variableTable = parmTable[index]
  local str = initStr .. " " .. variableTable.name .. " "
  for i, value in ipairs(variableTable.values) do
    local str2 = str .. value
    if index == #parmTable then
      table.insert(resultTable, str2)
    else
      RecurIterate(parmTable, resultTable, index + 1, str2)
    end
  end
end

function ReadLogFile(logFilePath)
  return dofile(logFilePath)
end

--Needs ImageMagick-6.7.3-6  with files renamed to IM. available at the path
function generateImageDiff(file1, file2, diffFile)
  os.execute("IM.convert " .. file1 .. " " .. file2 .. " -compose difference -composite -evaluate Pow 2 -evaluate divide 3 -separate -evaluate-sequence Add -evaluate Pow 0.5 " .. diffFile)
end

--Needs ImageMagick-6.7.3-6  with files renamed to IM. available at the path
function calcMetric(file1, file2, metric)
  metric = metric and metric or "PSNR"
  local tmpFile = os.tmpname()
  local tmpFile2 = os.tmpname()

  os.execute("IM.compare -metric " .. metric .. " " .. file1 .. " " .. file2 .. " " .. tmpFile .. " 2> " .. tmpFile2)
  
  retMetric = io.lines(tmpFile2)()
 
  -- os.execute("del " .. tmpFile)
  -- os.execute("del " .. tmpFile2)
  return retMetric, metric
 end
 
function writeHtml(htmlFileName, htmlTable)
  -- createFile
  local htmlFile = io.open(htmlFileName, "w")
  
  htmlFile:write("<html>\n")
  htmlFile:write("<body>\n")
  htmlFile:write("<table border  = \"1\">\n")
  
  htmlFile:write("<tr>\n")
    htmlFile:write("<td> ")
    htmlFile:write("DiffMetricMean")
    htmlFile:write("</td>\n")
    htmlFile:write("<td> ")
    htmlFile:write("TotalFrameTime(sec)")
    htmlFile:write("</td>\n")
    htmlFile:write("<td> ")
    htmlFile:write("FPS")
    htmlFile:write("</td>\n")
    for columnName, columnValue in pairs(htmlTable[1].algorithm_parameters) do
      htmlFile:write("<td> ")
      htmlFile:write(tostring(columnName))
      htmlFile:write("</td>\n")
    end
    htmlFile:write("<td> ")
    htmlFile:write("DiffLink")
    htmlFile:write("</td>\n")
  htmlFile:write("</tr>\n")
  
  for i, htmlRow in ipairs(htmlTable) do
    local diffLink = "htmlDiff"..tostring(i)..".html"
    createDiffFile(diffLink, htmlRow)
    
    htmlFile:write("<tr>\n")
      htmlFile:write("<td> ")
      htmlFile:write(htmlRow.metricMean)
      htmlFile:write("</td>\n")
      htmlFile:write("<td> ")
      htmlFile:write(htmlRow.time_tests.total_time)
      htmlFile:write("</td>\n")
      htmlFile:write("<td> ")
      htmlFile:write(1.0/htmlRow.time_tests.total_time)
      htmlFile:write("</td>\n")
      for columnName, columnValue in pairs(htmlRow.algorithm_parameters) do
        htmlFile:write("<td> ")
        htmlFile:write(tostring(columnValue))
        htmlFile:write("</td>\n")
      end
      htmlFile:write("<td> ")
      htmlFile:write("<a href=\""..diffLink.."\">Diff</a>")
      htmlFile:write("</td>\n")
    htmlFile:write("</tr>\n")
  end
    
  htmlFile:write("</table>\n")
  htmlFile:write("</body>\n")
  htmlFile:write("</html>\n")

  htmlFile:close()
end

function createDiffFile(htmlFileName, rowTable)
 local htmlFile = io.open(htmlFileName, "w")
  
  htmlFile:write("<html>\n")
  htmlFile:write("<body>\n")
  htmlFile:write("<table border  = \"1\">\n")
  htmlFile:write("<tr>\n")
  htmlFile:write("<td></td>\n")
  for i, screenShot in ipairs(rowTable.screenshots) do
    htmlFile:write("<td> ")
    htmlFile:write("ScrenShot"..tostring(i))
    htmlFile:write("</td>\n")
  end
  htmlFile:write("</tr>\n")
  
  htmlFile:write("<tr>\n")
  htmlFile:write("<td>Reference</td>\n")
  for i, screenShot in ipairs(rowTable.screenshots) do
    htmlFile:write("<td> ")
    htmlFile:write("<img src=".. screenShot.reference .." height=\"200\" />")
    htmlFile:write("</td>\n")
  end
  htmlFile:write("</tr>\n")
  
  htmlFile:write("<tr>\n")
  htmlFile:write("<td>Run</td>\n")
  for i, screenShot in ipairs(rowTable.screenshots) do
    htmlFile:write("<td> ")
    htmlFile:write("<img src=".. screenShot.img .." height=\"200\" />")
    htmlFile:write("</td>\n")
  end
  htmlFile:write("</tr>\n")  
  
  
  htmlFile:write("<tr>\n")
  htmlFile:write("<td>Diff</td>\n")
  for i, screenShot in ipairs(rowTable.screenshots) do
    htmlFile:write("<td> ")
    htmlFile:write("<img src=".. screenShot.diff .." height=\"200\" />")
    htmlFile:write("</td>\n")
  end
  htmlFile:write("</tr>\n")
  
    
  htmlFile:write("<tr>\n")
  htmlFile:write("<td>Metric</td>\n")
  for i, screenShot in ipairs(rowTable.screenshots) do
    htmlFile:write("<td> ")
    htmlFile:write(screenShot.diff_metric)
    htmlFile:write("</td>\n")
  end
  htmlFile:write("</tr>\n")
  
  htmlFile:write("</table>\n")
  htmlFile:write("</body>\n")
  htmlFile:write("</html>\n")

  htmlFile:close()
end
 
------------------------------
------------------------------
------------------------------
------------------------------
------------------------------
------------------------------
------------------------------
------------------------------
------------------------------
------------------------------
------------------------------

-- RecurIterate(parameters, args, 1, "")
args = {
"-scenepath resources/Scenes/scene.rt4 -benchmark True -logFile resources/Tests/logFileTmp.log -Cone_angle 1.04719755 -Cone_numCones 30 -Cone_numSpheres 8 -Cone_numSamplers 8 -Cone_infoMethod Progression -rfar  .8 -contrast  1.25 -jitter False",
"-scenepath resources/Scenes/scene.rt4 -benchmark True -logFile resources/Tests/logFileTmp.log -Cone_angle 1.04719755 -Cone_numCones 30 -Cone_numSpheres 8 -Cone_numSamplers 8 -Cone_infoMethod Progression -rfar  .8 -contrast  1.25 -jitter True"

}
htmlTable = {}
os.execute()
local workDirCmd = "cd " .. workingDir .. " & "
for i, arg in ipairs(args) do
  local cmdLine = workDirCmd .. "\"" .. programPath .. "\\" .. programName .. "\" " .. arg
  print(cmdLine)
  os.execute(cmdLine)
  local logtable = ReadLogFile(workingDir .. "\\" .. logFilePath)
  print(logtable.creation_date)
  
  local row = {
    arguments = arg,
    algorithm_parameters = logtable.algorithm_parameters,
    time_tests = logtable.time_tests,
    screenshots = {},
  }
  local metricSum = 0
  for j, screenShot in ipairs(logtable.screenshots) do
    local file1 = workingDir .. "\\" .. screenReference[j]
    local file2 = workingDir .. "\\" .. logtable.screenshots.path.."\\"..screenShot
    local diffFile = workingDir .. "\\" .. logtable.screenshots.path.."\\".."DIFF_"..screenShot
    print("Image Diff: ", file1, file2, diffFile)
    generateImageDiff(file1, file2, diffFile)
    local metric = calcMetric(file1,  file2)
    print(file1, file2, metric)
    metricSum = metricSum + metric
    table.insert(row.screenshots, {reference = file1, img = file2, diff = diffFile, diff_metric = metric})
  end
  row.metricMean = metricSum/(#logtable.screenshots)
  table.insert(htmlTable, row)
end


writeHtml(workingDir .. "\\" .. htmlFileName, htmlTable)


-- Rfar; Contrast; Jitter; ConeAngle; NumCones; NumSpheres; NumSamplers; CurrentInfoMethod; RadDistNumSpheres; RadDistSphereOverlap; RadProgSphereCenterParm; RadProgSphereRadiusParm;
-- 30.000; 20.000; Off; 0.523598776; 9; 9; 5; Progression; 10; 0.500; 3.000; 1.500;

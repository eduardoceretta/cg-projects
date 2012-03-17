-----------------------------------------------------------------------------------
-- Eduardo Ceretta Dalla Favera
-- eduardo.ceretta@gmail.com
-- 26 Feb 2012
-- 
-- Test the aplication. Varies the program input trough the command line and
-- scene file. Performs a series of tests anda generate a log file containing
-- the result of each test and the respectives screenshots.
-----------------------------------------------------------------------------------
executionEnabled = true

workingDir = "../../" -- from here

--DEBUG
-- programPath = "prj/ssao/vc9/testeSSAO_Debug/" -- from working dir
--RELEASE
programPath = "prj/ssao/vc9/testeSSAO_Release/" -- from working dir
programName = "testeSSAO.exe"

--Program Needed Paths
scenePath = "resources/Tests/testeScene.rt4"
shaderPath = "resources/Shaders/"

--Test Paths
logFilePath = "resources/Tests/Logs/"
-- logFileName = ""
screenShotsPath = "resources/Tests/ScreenShots/MainTest/"

--Enabled Tests
activeTests = {
  algorithmCompare = true,
  parameterVariation = true,
  scalability = true,
  geometryScalability = true,
  screenshotQuality = true,
  diffuseScreenshotQuality = true,
  timeTable = true,
}

--Program Aceptable Parameters
parameters = {
  scenepath = "-scenepath",
  shaderpath = "-shaderpath",
  
  logPath = "-logPath",
  logFileName = "-logFileName",
  screenShotsPath = "-screenShotsPath",

  screenShotTestEnabled  = "-test:screenshot",
  timeTestEnabled = "-test:time",

  sphereEnabled = "-alg:Sphere",
  rayMarchEnabled = "-alg:RayMarch",
  coneTracingEnabled = "-alg:ConeTracing",

  sphere_rfar = "-parm:Sphere:rfar",
  sphere_offset = "-parm:Sphere:offset",
  sphere_contrast = "-parm:Sphere:contrast",

  rayMarch_rfar = "-parm:RayMarch:rfar",
  rayMarch_contrast = "-parm:RayMarch:contrast",

  coneTracing_jitter = "-parm:ConeTracing:jitter",
  coneTracing_diffuseEnabled = "-parm:ConeTracing:diffuse_enabled",
  coneTracing_aoEnabled = "-parm:ConeTracing:ao_enabled",
  coneTracing_rfar = "-parm:ConeTracing:rfar",
  coneTracing_contrast = "-parm:ConeTracing:contrast",
  coneTracing_coneAngle = "-parm:ConeTracing:coneAngle",
  coneTracing_numCones = "-parm:ConeTracing:numCones",
  coneTracing_numSpheres = "-parm:ConeTracing:numSpheres",
  coneTracing_numSamplers = "-parm:ConeTracing:numSamplers",
}

-- Models Used in Tests. Sorted by importance. Not all will be used.
testModels = {
  path = "resources/Models/optixCompare/",
  {--1
    name = "cutter.msb",
    poses = {
      "4.1874 7.09915 151.792         4.1874 7.09915 -0.7609               0 1 0                           30       5      1000",
      "-99.3127 -14.3746 -33.4869     2.77799 8.8365 -1.57661              -0.127869 0.950722 -0.28245     30       5      1000",
      "105.553 23.2566 -36.3459       2.77799 8.8365 -1.57661              -0.212298 0.948763 -0.234048    30       5      1000",
    },
  },
  {--2
    name = "city.msb",
    poses = {
      "0.615725 0.236336 2.21997      0.615725 0.236336 -0.579334          0 1 0                            30       5      1000",
      "-2.41815 1.60309 0.720007      0.615725 0.236336 -0.579334          0.351855 0.923911 -0.150287      30       5      1000",
      "1.27591 0.72413 -2.41955       0.615725 0.236336 -0.579334          0.0543153 0.960192 0.274009      30       5      1000",
    },
  },
  {--3
    name = "DW-Ormesh-02.msb",
    poses = {
      "26.4077 180.487 1009.17        26.4077 180.487 -178.905             0 1 0                           30       5      1000",
    },
  },
  {--4
    name = "dragon.msb",
    poses = {
      "-21.5185  7.61081  18.0132  -8.46725e-008  4.96995  5.65807e-008  0.0502917  0.995042  -0.0858016 30 5 1000",
      "-12.9671  5.87109  -25.0109  -1.43579e-006  4.96995  -2.42542e-007  0.271954 0.956395  -0.106538  30 5 1000",
    },
  },
  {--5
    name = "ateneam.msb",
    poses = {
      "9395.73  1031.21  6815.89      -1311.34  873.505  25.674      -0.0161194  0.999868  0.00219707  30  5  1000",
      "2012.83  163.739  -1269.32      -1148.92  1750.66  316.511      0.368892  0.912364  -0.177512  30  5  1000",
    },
  },
  {--6
    name = "ducky.msb",
    poses = {
      "423.386  154.749  440.803      6.99708e-005  52.8394  -41.1333      -0.0947344  0.987548  -0.1256  30  5  1000",
      "-77.3621  148.57  190.565      45.8546  101.377  -32.59      0.031042  0.981222  -0.190369  30  5  1000",
    },
  },
  {--7
    name = "venusm.msb",
    poses = {
      "0.62915  1434.35  8945.67      0.62915  1434.35  -186.891      0  1  0  30  5  1000",
      "555.918  3948.94  944.666      32.0453  2893.19  47.401      -0.930649  0.203139  0.304345  30  5  1000",
    },
  },
  {--8
    name = "elepham.msb",
    poses = {
      "-1642.09  296.865  1036.14      187.186  467.413  110.326      -0.0938503  0.995584  -0.00203468  30  5  1000",
      "357.454  670.061  700.947      12.4391  497.878  226.295      -0.303574  0.944949  -0.122126  30  5  1000",
    },
  },   
  {--9
    name = "ladybird.msb",
    poses = {
      "1261.85  144.762  -508.307      0.42215  186.778  16.8873      0.00439427  0.997589  -0.0692558  30  5  1000",
      "83.9651  -275.241  -256.601      41.5013  -15.0654  -129.425      -0.00770198  0.438123  -0.898882  30  5  1000",
    },
  },    
  {--10
    name = "mba1.msb",
    poses = {
      -- "-9.30467  64.0636  416.061      -9.30467  64.0636  0.0113792      0  1  0  30  5  1000",
      "96.3126  89.7726  141.142      11.4312  68.186  -22.31      -0.0831427  0.992652  -0.0879209  30  5  1000",
    },
  },      
  {--11
    name = "donk.msb",
    poses = {
      "-74.9741  56.2721  54.43      2.5454  26.1191  1.26738      0.326969  0.94324  -0.0582187  30  5  1000",
      -- "-6.49261  23.9314  40.183      8.32765  35.5926  4.02966      0.134818  0.925554  0.3538  30  5  1000",
    },
  },  
  {--12
    name = "cornellbox.msb",
    poses = {
      "2.39071  2.76953  6.49835    2.54724  2.37149  -2.66228    -0.0133787  0.998958  -0.0436348          30       5      1000",
    },
  }, 
  {--13
    name = "bareliefply.msb",
    poses = {
      "13.3632  30.5817  3618.09     13.3632  30.5817  -138.15     0  1  0  30  5  1000",
      "3108.06  -377.201  1951.35     13.3632  30.5817  -138.151     -0.557155  0.0248511  0.830036  30  5  1000",
    },
  }, 
  {--14
    name = "brain.msb",
    poses = {
      "1.92317  13.7947  0.0336796 -0.153472  1.11954  -1.61502e-005 -0.986607  0.161582  0.0223143 30 5 1000",
      "8.31161  -0.0913365  3.99986  -1.99576  2.78985  -1.3377  0.218641  0.970497  0.101648 30 5 1000",

    },
  }, 
  {--15
    name = "buddha15M.msb",
    poses = {
      "0.445269  71.0018  318.843     0.445269  71.0018  -0.418875     0  1  0  30  5  1000",
      "-318.154  61.3621  17.7902     0.445272  71.0016  -0.418897     -0.0328169  0.998419  -0.045643  30  5  1000",
    },
  }, 
  {--16
    name = "bunzipper.msb",
    poses = {
      "-0.0168405  0.110154  0.343843     -0.0168405  0.110154  -0.001537     0  1  0  30  5  1000",
      "0.21772  0.276571  -0.192781     -0.0168405  0.110154  -0.00153703     -0.312391  0.872381  0.375983  30  5  1000",
    },
  }, 
  {--17
    name = "hand.msb",
    poses = {
      "5.34015  15.595  2.36007     3.40754  2.50787  1.54347     -0.229032  0.094274  -0.968843  30  5  1000",
      "-7.99303  -0.740623  -4.38534     3.40754  2.50787  1.54347     -0.117636  0.948667  -0.293587  30  5  1000",
    },
  }, 
  {--18
    name = "happyvrip.msb",
    poses = {
      "-0.005439  0.148768  0.389356     -0.005439  0.148768  -0.00669     0  1  0  30  5  1000",
      "0.18973  -0.0942517  0.0593348  -0.00623493  0.139666  -0.0290227  0.222726  0.502208  0.835572 30 5 1000",
    },
  }, 
  {--19
    name = "jsyrlin.msb",
    poses = {
      "56.3024  163.078  525.436     56.3024  163.078  -627.096     0  1  0  30  5  1000",
      "671.41  946.681  -47.4913     56.3024  163.078  -627.096     -0.296544  0.707371  -0.641629  30  5  1000",
    },
  }, 
  {--20
    name = "karburator-500k.msb",
    poses = {
      "208.383  -186.606  32.5288     -8.8237  -25.8593  -223.298     -0.0037459  0.845283  0.534306  30  5  1000",
      "-109.554  -315.481  -500.879     -8.8237  -25.8593  -223.298     0.354766  -0.708301  0.610287  30  5  1000",
    },
  }, 
  {--21
    name = "buddha15M.msb",
    poses = {
      "-420.733 28.6044 -436.562   0.445271 71.0018 -0.418875   420.484 42.4148 435.424   30 42.7269 1306",
      "-453.599 238.532 367.259   0.445271 71.0018 -0.418875   453.319 -167.171 -367.09   30 42.7269 1306",
    },
    multiple = {
      "50    80  0  1  1  1",
      "-50   80  0  1  1  1",
      "50   -80  0  1  1  1",
      "-50  -80  0  1  1  1",
    }
  }, 
  --PixelTest
  {--22 Bunny - 60.146k
    name = "bunzipper.msb",
    poses = {
      "0.125395 0.14234 0.436217   -0.0168404 0.110154 -0.00153695   0.162978 0.124248 0.501593   30 0.0221033 0.675614",
    },
  }, 
  {--23 Brain - 60.183k
    name = "brain.msb",
    poses = {
      "1.41285 1.11954 17.9032   -0.153471 1.11954 -1.62125e-005   -1.4795 0.0871557 -16.9108   30 0.911684 27.8667",
    },
  }, 
  {--24 Dragon - 60.148k
    name = "dragon.msb",
    poses = {
      "0.585739 4.96995 33.557   0 4.96995 0   -0.568353 0.0871557 -32.5609   30 2.00072 61.1545",
    },
  },   
  {--25 Jsyrlin - 60.183k
    name = "jsyrlin.msb",
    poses = {
      "165.513 163.078 934.683   56.3024 163.078 -627.096   -109.141 0.0871557 -1560.79   30 81.8076 2500.55",
    },
  }, 
  {--26 Hand - 60.124k
    name = "hand.msb",
    poses = {
      "4.62621 2.915 -10.0514   3.40754 2.50787 1.54347   -1.11429 -0.459468 10.6017   30 0.940796 28.7566",
    },
  },  
  {--27 Barelief - 60.145k
    name = "bareliefply.msb",
    poses = {
      "-201.018 784.824 6000.92   13.3632 30.5817 -138.15   214.347 -754.035 -6138.09   30 266.621 8149.6",
    },
  },   
  {--28 Happy - 60.120k
    name = "happyvrip.msb",
    poses = {
      "-0.0380124 0.154868 -0.416386   -0.00950909 0.147736 -0.00877069   -0.0408712 0.0973961 -0.584484   30 0.0281116 0.859267",
    },
  },    
  {--29 Buddha - 60.130k
    name = "buddha15M.msb",
    poses = {
      "7.31292 71.0018 393.028   0.445269 71.0018 -0.418875   -6.85026 0.0871557 -392.451   30 20.0131 611.725",
    },
  }, 
  
  
}


function createSceneFile(sceneName, resolution, models)
  local file = io.open(sceneName, "w")
  
  if file then
    file:write("RT 4.0\n")
    file:write("//File generated automatically!\n")
    --Scene
    file:write("SCENE 0.341 0.549 0.85 0.2 0.2 0.2 " .. tostring(resolution[1]) .. "  " .. resolution[2] .. "\n")
    --Default Camera
    file:write("CAMERA   100 100 40  0 0 0     0 1 0     30    5   1000 \n")
    --Material
    file:write("MATERIAL   .8 .8 .8     1. 1. 1.        50.0           0.0            0.8         0.4 \n")

    file:write("MATERIAL   .8 .8 .8     1. 1. 1.        50.0           0.0            0.8         0.4 \n")
    --Light
    -- file:write("LIGHT  .25 .5 .83   .85 .85 .85   .5 .5 .5\n\n")
    file:write("LIGHT  400 100 10   1 1 1   0 0 0\n\n")
    
    --Models
    local meshStr = "MESH 0 " 
    local transformStr = "0 0 0 1 1 1 " 
    local poseStr = "CAMERA "
    for i, model in ipairs(models) do
      if model.multiple == nil then
        local str = meshStr .. transformStr .. models.path
        file:write(str .. model.name .. " \n")
      else 
        for i, transformStr in ipairs(model.multiple) do
          local str = meshStr .. transformStr .. " " .. models.path
          file:write(str .. model.name .. " \n")
        end
      end
      --Cameras
      for j, pose in ipairs(model.poses) do
        file:write(poseStr .. pose .. "\n")
      end
      file:write("\n")
    end
    
    file:close()
    print("SceneFile " .. sceneName .. " Created")
  end
end

function runTestProgram(logFileName, scenePath, activeProgramTests, activeAlgorithms, algParameters)
  local workDirCmd = "cd " .. workingDir .. " & "
  local executablePath = "\"" .. programPath .. programName .. "\" "
  
  local args = parameters.shaderpath .. ' "' .. shaderPath .. '"   ' 
            .. parameters.scenepath .. ' "' .. scenePath .. '"   ' 
            .. parameters.logPath .. ' "' .. logFilePath .. '"   '
            .. parameters.logFileName .. ' "' .. logFileName .. '"   '
            .. parameters.screenShotsPath .. ' "' .. screenShotsPath .. '"   '
  for i, test in ipairs(activeProgramTests) do
    args = args .. test .. "   " 
  end
  
  for i, algorithm in ipairs(activeAlgorithms) do
    args = args .. algorithm .. "   " 
  end 
  
  for i, parms in ipairs(algParameters) do
    args = args .. parms[1] .. " " .. tostring(parms[2]) .. "   " 
  end
  
  local cmdLine = workDirCmd .. executablePath .. args
  print(cmdLine.."\n")
  
  if executionEnabled then
    os.execute(cmdLine)
  end
end

-- Tests
function algorithmCompareTest(resolution, models, parms)
  resolution = resolution and resolution or {640, 480}
  models = models and models or {path = testModels.path, testModels[14]} -- Brain
  parms = parms and parms or 
    {
      {parameters.sphere_contrast, 60.0},
      {parameters.rayMarch_rfar, 0.08228},
      {parameters.rayMarch_contrast, 3.24},
      --Media
      {parameters.coneTracing_contrast, 1.15,},
      {parameters.coneTracing_jitter, false,},
      {parameters.coneTracing_numSpheres, 5,},
      {parameters.coneTracing_numSamplers, 6,},
    }
  
  local tests = {
    parameters.timeTestEnabled,
    parameters.screenShotTestEnabled,
  }
  local algorithms = {
    parameters.sphereEnabled,
    parameters.rayMarchEnabled,
    parameters.coneTracingEnabled,
  }
    
  local sceneFullPath = workingDir..scenePath
  local logfileName = "AlgorithmCompareTest_"
  
  for i, model in ipairs(models) do
    local logName = logfileName .. model.name .. ".log"
    createSceneFile(sceneFullPath, resolution , {path = models.path, model})
    runTestProgram(logName, scenePath, tests, algorithms, parms)
    algorithmCompareLogAnaliser(logName)
  end
end

function parameterVariationTest(resolution, models)
  resolution = resolution and resolution or {640, 480}
  models = models and models or {path = testModels.path, testModels[10]}
  -- models = models and models or testModels
    
  local tests = {
    parameters.timeTestEnabled,
    parameters.screenShotTestEnabled,
  }
  local algorithms = {
    parameters.coneTracingEnabled,
  }
  local varyingParms = {
    {
      default = { --Média
      {parameters.coneTracing_contrast, 1.15,},
      {parameters.coneTracing_numSpheres, 5,},
      {parameters.coneTracing_numSamplers, 6,},
      },
      {parameters.coneTracing_jitter, {false, true},}
    },
  }
    
  local sceneFullPath = workingDir..scenePath
  local logfileName = "ParameterVariationTest_"
  
  
  for i, model in ipairs(models) do
    local logFileNameList = {}
    createSceneFile(sceneFullPath, resolution , {path = models.path, model})
    for j, varParms in ipairs(varyingParms) do
      for k, varParm in ipairs(varParms) do
        for l, varValue in ipairs(varParm[2]) do
          local parms = {}
          for m, defaultParm in ipairs(varParms.default) do
            table.insert(parms, defaultParm)
          end
          local logName = logfileName .. model.name .. "_" .. tostring(j) .. "_" .. tostring(k) .. "_" .. tostring(l) .. ".log"
          table.insert(parms, {varParm[1], varValue})
          runTestProgram(logName, scenePath, tests, algorithms, parms)
          table.insert(logFileNameList, logName)
        end
      end
    end
    parameterVariationLogAnaliser(logFileNameList)
  end
end

function scalabilityTest(models, parms)
  models = models and models or {path = testModels.path, testModels[1]}
  parms = parms and parms or 
      { --Média
        {parameters.coneTracing_contrast, 1.15,},
        {parameters.coneTracing_jitter, false,},
        {parameters.coneTracing_numSpheres, 5,},
        {parameters.coneTracing_numSamplers, 6,},
      }

  local tests = {
    parameters.timeTestEnabled,
  }
  local algorithms = {
    parameters.coneTracingEnabled,
  }
  
  local resolutions = {
    {640, 480},
    {800, 600},
    {1024, 768},
    {1280, 960},
    {1600, 1200},
    {2048, 1536},
  }
    
  local sceneFullPath = workingDir..scenePath
  local logfileName = "ScalabilityTest_"
  
  for i, model in ipairs(models) do
    local logFileNameList = {}
    for j, resolution in ipairs(resolutions) do
      local logName = logfileName .. model.name .. "_" .. tostring(j) .. ".log"
      createSceneFile(sceneFullPath, resolution , {path = models.path, model})
      runTestProgram(logName, scenePath, tests, algorithms, parms)
      table.insert(logFileNameList, logName)
    end
    scalabilityLogAnaliser(logFileNameList)
  end
end

function geometryScalabilityTest(models, parms)
  resolution = resolution and resolution or {640, 480}
  models = models and models or {path = testModels.path, 
      testModels[22], 
      -- testModels[23], 
      -- testModels[24], 
      testModels[25], 
      testModels[26], 
      testModels[27], 
      testModels[28], 
      testModels[29] 
    }
  parms = parms and parms or 
    { --Média
      {parameters.coneTracing_contrast, 1.15,},
      {parameters.coneTracing_jitter, false,},
      {parameters.coneTracing_numSpheres, 5,},
      {parameters.coneTracing_numSamplers, 6,},
    }

  local tests = {
    parameters.timeTestEnabled,
    parameters.screenShotTestEnabled,
  }
  local algorithms = {
    parameters.coneTracingEnabled,
  }
  
  local sceneFullPath = workingDir..scenePath
  local logfileName = "GeometryScalabilityTest_"
  
  local logFileNameList = {}
  for i, model in ipairs(models) do
    local logName = logfileName .. model.name .. "_" .. tostring(i) .. ".log"
    createSceneFile(sceneFullPath, resolution , {path = models.path, model})
    runTestProgram(logName, scenePath, tests, algorithms, parms)
    table.insert(logFileNameList, logName)
  end
  geometryScalabilityLogAnaliser(logFileNameList)
end

function screenshotQualityTest(resolution, models)
  resolution = resolution and resolution or {640, 480}
  models = models and models or {path = testModels.path, 
    testModels[4],
    -- testModels[12],
    -- testModels[13],
    testModels[14],
    testModels[15],
    -- testModels[16],
    testModels[17],
    testModels[18],
    testModels[19],
    testModels[20],
  }
    
    
  local tests = {
    parameters.timeTestEnabled,
    parameters.screenShotTestEnabled,
  }
  local algorithms = {
    parameters.coneTracingEnabled,
  }
  local varyingParms = {
    {
      name = "Baixa", 
      {parameters.coneTracing_contrast, 1.4,},
      {parameters.coneTracing_jitter, false,},
      {parameters.coneTracing_numSpheres, 3,},
      {parameters.coneTracing_numSamplers, 3,},
    },    
    {
      name = "Media",
      {parameters.coneTracing_contrast, 1.15,},
      {parameters.coneTracing_jitter, false,},
      {parameters.coneTracing_numSpheres, 5,},
      {parameters.coneTracing_numSamplers, 6,},
    },    
    {
      name = "Alta",
      {parameters.coneTracing_contrast, 1.05,},
      {parameters.coneTracing_jitter, false,},
      {parameters.coneTracing_numSpheres, 7,},
      {parameters.coneTracing_numSamplers, 12,},
    },
  }
    
  local sceneFullPath = workingDir..scenePath
  local logfileName = "ScreenshotQualityTest_"
  
  for i, model in ipairs(models) do
    createSceneFile(sceneFullPath, resolution , {path = models.path, model})
    local logFileNameList = {}
    for j, parms in ipairs(varyingParms) do
      local logName = logfileName .. model.name .. "_" .. parms.name .. ".log"
      runTestProgram(logName, scenePath, tests, algorithms, parms)
      table.insert(logFileNameList, logName)
    end
    screenshotQualityLogAnaliser(logFileNameList)
  end
end

function diffuseScreenshotQualityTest(resolution, models)
  resolution = resolution and resolution or {1024, 768}
  models = models and models or {path = testModels.path, 
    testModels[21],
  }
    
    
  local tests = {
    parameters.screenShotTestEnabled,
  }
  local algorithms = {
    parameters.coneTracingEnabled,
  }
  
  local varyingParms = {
    {
      -- name = "Iluminação Difusa desligada e Oclusão Ambiente ligada", 
      name = "DifOff_AoOn", 
      {parameters.coneTracing_diffuseEnabled, false,},
      {parameters.coneTracing_aoEnabled, true,},
      {parameters.coneTracing_contrast, 1.15,},
      {parameters.coneTracing_jitter, false,},
      {parameters.coneTracing_numSpheres, 5,},
      {parameters.coneTracing_numSamplers, 6,},
    },  
    {
      -- name = "Iluminação Difusa Ligada e Oclusão Ambiente Ligada", 
      name = "DifOn_AoOn", 
      {parameters.coneTracing_diffuseEnabled, true,},
      {parameters.coneTracing_aoEnabled, true,},
      {parameters.coneTracing_contrast, 1.15,},
      {parameters.coneTracing_jitter, false,},
      {parameters.coneTracing_numSpheres, 5,},
      {parameters.coneTracing_numSamplers, 6,},
    },    
    {
      -- name = "Iluminação Difusa Ligada e Oclusão Ambiente desligada", 
      name = "DifOn_AoOff", 
      {parameters.coneTracing_diffuseEnabled, true,},
      {parameters.coneTracing_aoEnabled, false,},
      {parameters.coneTracing_contrast, 1.15,},
      {parameters.coneTracing_jitter, false,},
      {parameters.coneTracing_numSpheres, 5,},
      {parameters.coneTracing_numSamplers, 6,},
    },    
  }
    
  local sceneFullPath = workingDir..scenePath
  local logfileName = "DiffuseScreenshotQualityTest_"
  
  for i, model in ipairs(models) do
    createSceneFile(sceneFullPath, resolution , {path = models.path, model})
    local logFileNameList = {}
    for j, parms in ipairs(varyingParms) do
      local logName = logfileName .. model.name .. "_" .. parms.name .. ".log"
      runTestProgram(logName, scenePath, tests, algorithms, parms)
      table.insert(logFileNameList, logName)
    end
    diffuseScreenshotQualityLogAnaliser(logFileNameList)
  end
end

function timeTableTest(resolution, models, parms)
  resolution = resolution and resolution or {640, 480}
  models = models and models or {path = testModels.path, testModels[21]}
  parms = parms and parms or 
    {
      {parameters.coneTracing_contrast, 1.15,},
      {parameters.coneTracing_jitter, false,},
      {parameters.coneTracing_numSpheres, 5,},
      {parameters.coneTracing_numSamplers, 6,},
    }
  
  
  local tests = {
    parameters.timeTestEnabled,
    -- parameters.screenShotTestEnabled,
  }
  local algorithms = {
    parameters.coneTracingEnabled,
  }
  local sceneFullPath = workingDir..scenePath
  local logfileName = "TimeTableTest_"
  
  for i, model in ipairs(models) do
    local logName = logfileName .. model.name .. ".log"
    createSceneFile(sceneFullPath, resolution , {path = models.path, model})
    runTestProgram(logName, scenePath, tests, algorithms, parms)
    timeTableLogAnaliser(logName)
  end
end


-- Test Log Analiser
function algorithmCompareLogAnaliser(logFileName)
  local testName = "AlgorithmCompare"
  local timeMetric = "Mean" -- {"Mean" | "Single"}
  local logFile = loadfile(workingDir .. logFilePath .. logFileName)
  if logFile then
    local logFileTable = logFile()
    local modelName = logFileTable.scene.models[1][1]
    
    -- Create Result Log Dir
    local analiseLogDir = testName .. "_" .. modelName .. "_analysis"
    local cmdStr = string.gsub("mkdir " .. workingDir .. logFilePath .. analiseLogDir, "/", "\\")
    os.execute(cmdStr)
    
    -- Create Result Log Figure Dir
    local figuresDir = "figures"
    local cmdStr = string.gsub("mkdir " .. workingDir .. logFilePath .. analiseLogDir .. "/" .. figuresDir, "/", "\\")
    os.execute(cmdStr)
    
    -- Create Result Log File
    local texFileName = testName .. "_analysis.tex"
    local texFile = io.open(workingDir .. logFilePath .. analiseLogDir .. "/" .. texFileName, "w")
    
    --Intro
    texFile:write("%Tabela de comparação de tempo por algoritmo.\n\n")
    texFile:write("%Gerada em " .. logFileTable.creation_date .. "\n\n")
    texFile:write("\n")
    
    --Algorithms
    for i, alg in ipairs(logFileTable.algorithms) do
      texFile:write("%Algoritmo utilizado: " .. alg.name .. "\n\n")
      texFile:write("%Os parâmetros de execução são: \n")
      texFile:write("%\\begin{itemize}\n")
      for j, parm in ipairs(alg) do
        texFile:write("%  \\item " .. tostring(parm[1]) .. " = " .. tostring(parm[2]) .. "\n")
      end
      texFile:write("%\\end{itemize}\n")
    end
    texFile:write("\n\n")
    
    --Scene
    texFile:write("%A cena é composta pelo modelo ".. modelName ..".".."\n\n")
    texFile:write("A cena utilizada neste teste é composta pelo modelo \\emph{" .. modelName .. "} e possui " .. math.floor(logFileTable.scene.totalVertices/1000) .. " mil vértices e ".. math.floor(logFileTable.scene.totalTriangles/1000)  .. " mil triângulos.\n\n")
    texFile:write("%A resolução utilizada é ".. logFileTable.resolution[1] .."x" .. logFileTable.resolution[2].."\n\n")
    texFile:write("\n")

    -- Move Figures
    for i, alg in ipairs(logFileTable.algorithms) do
      for j, pose in ipairs(logFileTable[alg.name]) do
        local cmdLine = string.gsub("copy \"" .. workingDir .. pose.screenshotTest.screenshotPath .. pose.screenshotTest.screenshotFile .. "\"  "   
                                    .. workingDir .. logFilePath .. analiseLogDir .. "/" .. figuresDir .. "/" .. pose.screenshotTest.screenshotFile, "/", "\\")
        os.execute(cmdLine)
      end
    end
    
    -- SubFloats 
    texFile:write("\\begin{figure}[H]\n")
    texFile:write("  \\centering\n")
    
    for i, alg in ipairs(logFileTable.algorithms) do
      for j, pose in ipairs(logFileTable[alg.name]) do
        local n = #logFileTable[alg.name]
        local size = math.floor(.5 + 100/n)/100
        texFile:write("  \\subfloat{\\includegraphics[width=".. tostring(size) .."\\textwidth]{".. figuresDir .. "/" .. pose.screenshotTest.screenshotFile .. "} }" .. (j == n and "\\\\" or "") .. "\n")
      end
      texFile:write("  " .. alg.name .. " \\\\ \n")
      texFile:write("  \\vspace{15pt} \n")
    end
    
    for j, pose in ipairs(logFileTable.poses) do
      local n = #logFileTable.poses
      local size = math.floor(.5 + 100/n)/100
      texFile:write("  \\subfloat{\\includegraphics[width=".. tostring(size) .."\\textwidth]{".. figuresDir .. "/" .. modelName .. pose .. ".png" .. "} }" .. (j == n and "\\\\" or "") .. "\n")
    end
    texFile:write("  Optix \\\\ \n")

    texFile:write("  \\caption{Comparação entre os algoritmos.}\n")
    texFile:write("  \\label{fig:".. testName .."}\n")
    texFile:write("\\end{figure}\n")
    texFile:write("\n")
    texFile:write("\n")
    
    
    --TableLines
    local tableLines = {}
    for i, alg in ipairs(logFileTable.algorithms) do
      local tableLine = alg.name
      for j, pose in ipairs(logFileTable[alg.name]) do
        tableLine = tableLine .. " & " .. tostring(math.floor(pose.timeTest.total.GPU[timeMetric]*100+.5)/100) .. " ms"
      end
      tableLine = tableLine .. " \\\\ \n"
      for j, pose in ipairs(logFileTable[alg.name]) do
        local ms = math.floor(pose.timeTest.total.GPU[timeMetric]*100+.5)/100
        local fps = math.floor((1.0/(ms/1000.0))*100 + .5)/100
        tableLine = tableLine .. " & " .. tostring(fps) .. " fps"
      end
      table.insert(tableLines, tableLine)
    end
    
    -- Table
    local columns = " c |"
    for i, j in ipairs(logFileTable.poses) do
      columns = columns .. " c |"
    end
    
    texFile:write("%A tabela a seguir apresenta os resultados obtidos para cada algoritmo." .. "\n")
    texFile:write("\\begin{table}[H]".."\n")
    texFile:write("  \\centering".."\n")
    texFile:write("  \\caption{Resultados de tempo para cada aloritmo.}".."\n")
    texFile:write("  \\label{tab:".. testName .."}\n")
    texFile:write("  \\begin{tabular}{ |"..columns.."}".."\n")
    texFile:write("    \\hline".."\n")
    --Header
    texFile:write("  Algoritmo")
    for i, pose in ipairs(logFileTable.poses) do
      texFile:write(" & Pose " .. i .. " " )
    end
    texFile:write("\\\\ \n")
    texFile:write("    \\hline".."\n")
    
    --Content
    for i, tabLine in ipairs(tableLines) do
      texFile:write("    " .. tabLine .. " \\\\ \n")
      texFile:write("    \\hline".."\n")
    end
    
    texFile:write("  \\end{tabular}".."\n")
    texFile:write("\\end{table}".."\n")
    texFile:write("\n")
    texFile:close()
  end
end

function parameterVariationLogAnaliser(logFileNameList)
  local testName = "ParameterVariation"
  local timeMetric = "Mean" -- {"Mean" | "Single"}
  
  local logFileList = {}
  for i, logFileName in ipairs(logFileNameList) do
    local logFile = loadfile(workingDir .. logFilePath .. logFileName)
    table.insert(logFileList, logFile)
  end
  
  local logFileTableList = {}
  for i, logFile in ipairs(logFileList) do
    local logFileTable = logFile()
    table.insert(logFileTableList, logFileTable)
  end  
  
  local logFileTable = logFileTableList[1]
  local modelName = logFileTable.scene.models[1][1]
    
  -- Create Result Log Dir
  local analiseLogDir = testName .. "_" .. modelName .. "_analysis"
  local cmdStr = string.gsub("mkdir " .. workingDir .. logFilePath .. analiseLogDir, "/", "\\")
  os.execute(cmdStr)
    
  -- Create Result Log Figure Dir
  local figuresDir = "figures"
  local cmdStr = string.gsub("mkdir " .. workingDir .. logFilePath .. analiseLogDir .. "/" .. figuresDir, "/", "\\")
  os.execute(cmdStr)
    
  -- Create Result Log File
  local texFileName = testName .. "_analysis.tex"
  local texFile = io.open(workingDir .. logFilePath .. analiseLogDir .. "/" .. texFileName, "w")
  
  --Intro
  texFile:write("%Tabela de comparação de tempo de acordo com cada parâmetro.\n\n")
  texFile:write("%Gerada em " .. logFileTable.creation_date .. "\n\n")
  texFile:write("\n")
  
  --Algorithms
  for i, alg in ipairs(logFileTable.algorithms) do
    texFile:write("%Algoritmo utilizado: " .. alg.name .. "\n\n")
  end
  texFile:write("\n")
    
  --Scene
  texFile:write("%A cena é composta pelo modelo ".. modelName ..".".."\n\n")
  texFile:write("A cena utilizada neste teste é composta pelo modelo \\emph{" .. modelName .. "} e possui " .. math.floor(logFileTable.scene.totalVertices/1000) .. " mil vértices e ".. math.floor(logFileTable.scene.totalTriangles/1000)  .. " mil triângulos.\n\n")
  texFile:write("%A resolução utilizada é ".. logFileTable.resolution[1] .."x" .. logFileTable.resolution[2].."\n\n")
  texFile:write("\n")

  -- Move Figures
  for i, logFileTable in ipairs(logFileTableList) do
    for j, pose in ipairs(logFileTable[logFileTable.algorithms[1].name]) do
      local cmdLine = string.gsub("copy \"" .. workingDir .. pose.screenshotTest.screenshotPath .. pose.screenshotTest.screenshotFile .. "\"  "   
                                  .. workingDir .. logFilePath .. analiseLogDir .. "/" .. figuresDir .. "/" .. pose.screenshotTest.screenshotFile, "/", "\\")
      os.execute(cmdLine)
    end
  end
  
  --Instance Text
  local nameLines = {"\\emph{Jitter} desativado","\\emph{Jitter} ativado"}
  -- local parmList = {3, 6, 7} --jitter, numSpheres, numSamplers
  -- for i, logFileTable in ipairs(logFileTableList) do
    -- local alg = logFileTable.algorithms[1]
    -- local instanceLine = ""
    -- for j, parmIndex in ipairs(parmList) do
      -- instanceLine = instanceLine .. alg[parmIndex][1] .. "(" .. alg[parmIndex][2] .. ")" .. (j ~= #parmList and ", " or "")
    -- end
    -- table.insert(nameLines, instanceLine)
  -- end
  
  -- SubFloats 
  for i, logFileTable in ipairs(logFileTableList) do
    texFile:write("\\begin{figure}[H]\n")
    texFile:write("  \\centering\n")
    local alg = logFileTable.algorithms[1]
    local pose = logFileTable[alg.name][1]
    local n = #logFileTable[alg.name]
    local size = math.min(math.floor(.5 + 100/n)/100, .5)
    texFile:write("  \\subfloat{\\includegraphics[width=".. tostring(size) .."\\textwidth]{".. figuresDir .. "/" .. pose.screenshotTest.screenshotFile .. "} }" .. "\n")
    texFile:write("\\\\  " .. nameLines[i] .. " \n")
    texFile:write("\\end{figure}\n")
    texFile:write("\n")
  end
  
  texFile:write("\\begin{figure}[H]\n")
  texFile:write("  \\centering\n")
  local pose = logFileTable.poses[1]
  local n = #logFileTable.poses
  local size = math.min(math.floor(.5 + 100/n)/100, .5)
  texFile:write("  \\subfloat{\\includegraphics[width=".. tostring(size) .."\\textwidth]{".. figuresDir .. "/" .. modelName .. pose .. ".png" .. "} }" .. "\n")
  texFile:write("\\\\    Optix \n")
  texFile:write("    \\caption{Comparação entre execução com e sem \\emph{jitter}.} \n")
  texFile:write("    \\label{fig:".. testName .."}\n")
  texFile:write("\\end{figure}\n")
  texFile:write("\n")
  texFile:write("\n")
  
  
  --TableLines
  local tableLines = {}
  for i, logFileTable in ipairs(logFileTableList) do
    local alg = logFileTable.algorithms[1]
    local tableLine = nameLines[i]
    local pose = logFileTable[logFileTable.algorithms[1].name][1]
    local ms = math.floor(pose.timeTest.total.GPU[timeMetric]*100+.5)/100
    local fps = math.floor((1.0/(ms/1000.0))*100 + .5)/100
    tableLine = tableLine .. " & " .. tostring(ms)  
    tableLine = tableLine .. " & " .. tostring(fps) 
    table.insert(tableLines, tableLine)
  end
 
  
  --Table
  local columns = " c | c | c |"
  -- for i, j in ipairs(logFileTable.poses) do
    -- columns = columns .. " c |"
  -- end
  
  texFile:write("%A tabela a seguir apresenta os resultados obtidos para cada varialão de jitter." .. "\n")
  texFile:write("\\begin{table}[H]".."\n")
  texFile:write("  \\centering".."\n")
  texFile:write("  \\caption{Resultados de tempo para a execução com e sem \\emph{jitter}.}".."\n")
  texFile:write("  \\label{tab:".. testName .."}\n")
  texFile:write("  \\begin{tabular}{ |"..columns.."}".."\n")
  texFile:write("    \\hline".."\n")
  --Header
  texFile:write("    Parâmetros & Tempo (ms) & FPS")

  texFile:write("\\\\ \n")
  texFile:write("    \\hline".."\n")
  
  --Content
  for i, tabLine in ipairs(tableLines) do
    texFile:write("    " .. tabLine .. " \\\\ \n")
    texFile:write("    \\hline".."\n")
  end
  
  texFile:write("  \\end{tabular}".."\n")
  texFile:write("\\end{table}".."\n")
  texFile:write("\n")
  texFile:close()
end

function scalabilityLogAnaliser(logFileNameList)
  local testName = "Scalability"
  local timeMetric = "Mean" -- {"Mean" | "Single"}
  
  local logFileList = {}
  for i, logFileName in ipairs(logFileNameList) do
    local logFile = loadfile(workingDir .. logFilePath .. logFileName)
    table.insert(logFileList, logFile)
  end
  
  local logFileTableList = {}
  for i, logFile in ipairs(logFileList) do
    local logFileTable = logFile()
    table.insert(logFileTableList, logFileTable)
  end  
  
  local logFileTable = logFileTableList[1]
  local modelName = logFileTable.scene.models[1][1]
    
  -- Create Result Log Dir
  local analiseLogDir = testName .. "_" .. modelName .. "_analysis"
  local cmdStr = string.gsub("mkdir " .. workingDir .. logFilePath .. analiseLogDir, "/", "\\")
  os.execute(cmdStr)
    
  -- Create Result Log File
  local texFileName = testName .. "_analysis.tex"
  local texFile = io.open(workingDir .. logFilePath .. analiseLogDir .. "/" .. texFileName, "w")
    
  --Intro
  texFile:write("%Tabela de comparação de tempo de acordo com cada resolução.\n\n")
  texFile:write("%Gerada em " .. logFileTable.creation_date .. "\n\n")
  texFile:write("\n")
  
  --Algorithms
  for i, alg in ipairs(logFileTable.algorithms) do
    texFile:write("%Algoritmo utilizado: " .. alg.name .. "\n\n")
  end
  texFile:write("\n")
    
  --Scene
  texFile:write("%A cena é composta pelo modelo \\emph{".. modelName .."}.".."\n\n")
  texFile:write("A cena utilizada neste teste é composta pelo modelo \\emph{" .. modelName .. "} e possui " .. math.floor(logFileTable.scene.totalVertices/1000) .. " mil vértices e ".. math.floor(logFileTable.scene.totalTriangles/1000)  .. " mil triângulos.\n\n")
  texFile:write("\n")
  
  --TableLines
  local csvTableLines = {}
  local parameterTableLines = {}
  
  for i, logFileTable in ipairs(logFileTableList) do
    local alg = logFileTable.algorithms[1]
    local pose = logFileTable[alg.name][1]
    local tableLine = logFileTable.resolution[1] .. "x" .. logFileTable.resolution[2]
    local csvLine = "%%" .. logFileTable.resolution[1]*logFileTable.resolution[2]
    
    local ms = math.floor(pose.timeTest.total.GPU[timeMetric]*100+.5)/100
    local fps = math.floor((1.0/(ms/1000.0))*100 + .5)/100
    tableLine = tableLine .. " & " .. tostring(ms) 
    csvLine = csvLine .. "\t" .. tostring(ms)
    tableLine = tableLine .. " & " .. tostring(fps)
    
    table.insert(parameterTableLines, tableLine)
    table.insert(csvTableLines, csvLine)
  end

  --Table
  local columns = " c | c | c |"

  texFile:write("A tabela a seguir apresenta os resultados obtidos para cada resolução." .. "\n")
  texFile:write("\\begin{table}[H]".."\n")
  texFile:write("  \\centering".."\n")
  texFile:write("  \\caption{Resultados de tempo para a execução em diversas resoluções.}\n")
  texFile:write("  \\label{tab:".. testName .."}\n")
  texFile:write("  \\begin{tabular}{ |"..columns.."}".."\n")
  texFile:write("    \\hline".."\n")
  --Header
  texFile:write("    Resolução & Tempo (ms) & FPS \\\\ \n")
  texFile:write("    \\hline".."\n")
  
  --Content
  for i, parameterLine in ipairs(parameterTableLines) do
    texFile:write("    " .. parameterLine .. " \\\\ \n")
    texFile:write("    \\hline".."\n")
  end
  
  texFile:write("  \\end{tabular}".."\n")
  texFile:write("\\end{table}".."\n")
  
  texFile:write("%%CSV Data".."\n")
  
  for i, csvLine in ipairs(csvTableLines) do
    texFile:write(csvLine .. "\n")
  end
  
  texFile:close()
end

function geometryScalabilityLogAnaliser(logFileNameList)
  local testName = "GeometryScalability"
  local timeMetric = "Mean" -- {"Mean" | "Single"}
  
  local logFileList = {}
  for i, logFileName in ipairs(logFileNameList) do
    local logFile = loadfile(workingDir .. logFilePath .. logFileName)
    table.insert(logFileList, logFile)
  end
  
  local logFileTableList = {}
  for i, logFile in ipairs(logFileList) do
    local logFileTable = logFile()
    table.insert(logFileTableList, logFileTable)
  end  
  
  local logFileTable = logFileTableList[1]
  -- local modelName = logFileTable.scene.models[1][1]
    
  -- Create Result Log Dir
  local analiseLogDir = testName .. "_analysis"
  local cmdStr = string.gsub("mkdir " .. workingDir .. logFilePath .. analiseLogDir, "/", "\\")
  os.execute(cmdStr)
  
  -- Create Result Log Figure Dir
  local figuresDir = "figures"
  local cmdStr = string.gsub("mkdir " .. workingDir .. logFilePath .. analiseLogDir .. "/" .. figuresDir, "/", "\\")
  os.execute(cmdStr)
    
  -- Create Result Log File
  local texFileName = testName .. "_analysis.tex"
  local texFile = io.open(workingDir .. logFilePath .. analiseLogDir .. "/" .. texFileName, "w")
    
  --Intro
  texFile:write("%Tabela de comparação de tempo de acordo com cada modelo.\n\n")
  texFile:write("%Gerada em " .. logFileTable.creation_date .. "\n\n")
  texFile:write("\n")
  
  --Algorithms
  for i, alg in ipairs(logFileTable.algorithms) do
    texFile:write("%Algoritmo utilizado: " .. alg.name .. "\n\n")
  end
  texFile:write("\n")
  
  -- Move Figures
  for i, logFileTable in ipairs(logFileTableList) do
    for j, pose in ipairs(logFileTable[logFileTable.algorithms[1].name]) do
      local cmdLine = string.gsub("copy \"" .. workingDir .. pose.screenshotTest.screenshotPath .. pose.screenshotTest.screenshotFile .. "\"  "   
                                  .. workingDir .. logFilePath .. analiseLogDir .. "/" .. figuresDir .. "/" .. pose.screenshotTest.screenshotFile, "/", "\\")
      os.execute(cmdLine)
    end
  end
    
  
  --TableLines
  local csvTableLines = {}
  local parameterTableLines = {}
  
  for i, logFileTable in ipairs(logFileTableList) do
    local alg = logFileTable.algorithms[1]
    local pose = logFileTable[alg.name][1]
    local modelName = logFileTable.scene.models[1][1]
    local modelNumVertices = math.floor((logFileTable.scene.totalVertices/1000)*100 + .5)/100
    local modelNumTriangles = math.floor((logFileTable.scene.totalTriangles/1000)*100 + .5)/100
    local ms = math.floor(pose.timeTest.total.GPU[timeMetric]*100+.5)/100
    local fps = math.floor((1.0/(ms/1000.0))*100 + .5)/100

    local tableLine = modelName 
    local csvLine = "%%" .. modelName 
    tableLine = tableLine .. " & " .. modelNumVertices
    csvLine =  csvLine .. "\t" .. modelNumVertices    
    tableLine = tableLine .. " & " .. modelNumTriangles
    csvLine =  csvLine .. "\t" .. modelNumTriangles
    tableLine = tableLine .. " & " .. tostring(ms) 
    csvLine =  csvLine .. "\t" .. tostring(ms) 
    tableLine = tableLine .. " & " .. tostring(fps)

    table.insert(parameterTableLines, tableLine)
    table.insert(csvTableLines, csvLine)
  end

  --Table
  local columns = " c | c | c | c | c |"

  texFile:write("A tabela a seguir apresenta o tempo para a execução de cada modelo de acordo com o seu número de triângulos." .. "\n")
  texFile:write("\\begin{table}[H]".."\n")
  texFile:write("  \\centering".."\n")
  texFile:write("  \\caption{Resultados de tempo para a execução de modelos de diversos tamanhos.}\n")
  texFile:write("  \\label{tab:".. testName .."}\n")
  texFile:write("  \\begin{tabular}{ |"..columns.."}".."\n")
  texFile:write("    \\hline".."\n")
  --Header
  texFile:write("    Modelo & Vértices (mil) & Triângulos (mil) & Tempo (ms) & FPS \\\\ \n")
  texFile:write("    \\hline".."\n")
  
  --Content
  for i, parameterLine in ipairs(parameterTableLines) do
    texFile:write("    " .. parameterLine .. " \\\\ \n")
    texFile:write("    \\hline".."\n")
  end
  
  texFile:write("  \\end{tabular}".."\n")
  texFile:write("\\end{table}".."\n")
  
  texFile:write("%%CSV Data".."\n")
  
  for i, csvLine in ipairs(csvTableLines) do
    texFile:write(csvLine .. "\n")
  end
  
  texFile:close()
end

function screenshotQualityLogAnaliser(logFileNameList)
  local testName = "ScreenshotQuality"
  local timeMetric = "Mean" -- {"Mean" | "Single"}
  local nameLines = {"Baixa", "Média", "Alta"}
  
  local logFileList = {}
  for i, logFileName in ipairs(logFileNameList) do
    local logFile = loadfile(workingDir .. logFilePath .. logFileName)
    table.insert(logFileList, logFile)
  end
  
  local logFileTableList = {}
  for i, logFile in ipairs(logFileList) do
    local logFileTable = logFile()
    table.insert(logFileTableList, logFileTable)
  end  
  
  local logFileTable = logFileTableList[1]
  local modelName = logFileTable.scene.models[1][1]
    
  -- Create Result Log Dir
  local analiseLogDir = testName .. "_" .. modelName .. "_analysis"
  local cmdStr = string.gsub("mkdir " .. workingDir .. logFilePath .. analiseLogDir, "/", "\\")
  os.execute(cmdStr)
    
  -- Create Result Log Figure Dir
  local figuresDir = "figures"
  local cmdStr = string.gsub("mkdir " .. workingDir .. logFilePath .. analiseLogDir .. "/" .. figuresDir, "/", "\\")
  os.execute(cmdStr)
    
  -- Create Result Log File
  local texFileName = testName .. "_" .. modelName .. "_analysis.tex"
  local texFile = io.open(workingDir .. logFilePath .. analiseLogDir .. "/" .. texFileName, "w")

  texFile:write("\\subsection{Modelo ".. modelName .."}\n\n")
  --Intro
  texFile:write("%Tabela de comparação qualidade.\n\n")
  texFile:write("%Gerada em " .. logFileTable.creation_date .. "\n\n")
  texFile:write("\n")
  
  --Algorithms
  for i, alg in ipairs(logFileTable.algorithms) do
    texFile:write("%Algoritmo utilizado: " .. alg.name .. "\n\n")
  end
  texFile:write("\n")
    
  --Scene
  texFile:write("%A cena é composta pelo modelo ".. modelName ..".".."\n\n")
  texFile:write("%A resolução utilizada é ".. logFileTable.resolution[1] .."x" .. logFileTable.resolution[2].."\n\n")
  texFile:write("A cena utilizada neste teste é composta pelo modelo \\emph{" .. modelName .. "} e possui " .. math.floor(logFileTable.scene.totalVertices/1000) .. " mil vértices e ".. math.floor(logFileTable.scene.totalTriangles/1000)  .. " mil triângulos. ")
  texFile:write("A Tabela \\ref{tab:".. testName .."-" .. modelName .. "} contém os resultados de tempo para a execução de cada uma das configurações. ")
  texFile:write("As imagens na Figura \\ref{fig:".. testName .."-"..modelName.."} visam apresentar uma comparação entre a qualidade de cada uma das configurações.\n\n")
  texFile:write("\n")
  
  
  
  
  --TableLines
  local parameterTableLines = {}
  for i, logFileTable in ipairs(logFileTableList) do
    local alg = logFileTable.algorithms[1]
    local tableLine = nameLines[i]
    for j, pose in ipairs(logFileTable[logFileTable.algorithms[1].name]) do
      tableLine = tableLine .. " & " .. tostring(math.floor(pose.timeTest.total.GPU[timeMetric]*100+.5)/100) .. " ms"
    end
    tableLine = tableLine .. " \\\\ \n"
    for j, pose in ipairs(logFileTable[logFileTable.algorithms[1].name]) do
      local ms = math.floor(pose.timeTest.total.GPU[timeMetric]*100+.5)/100
      local fps = math.floor((1.0/(ms/1000.0))*100 + .5)/100
      tableLine = tableLine .. " & " .. tostring(fps) .. " fps"
    end
    table.insert(parameterTableLines, tableLine)
  end

  -- Table
  local columns = " p{2cm} |"
  for i, j in ipairs(logFileTable.poses) do
    columns = columns .. " c |"
  end
  
  texFile:write("%A tabela a seguir apresenta os resultados obtidos para cada configuração de qualidade." .. "\n")
  texFile:write("\\begin{table}[H]".."\n")
  texFile:write("  \\centering".."\n")
  texFile:write("  \\caption{Comparação entre execuções de diferentes qualidades do modelo " .. modelName .. ".}".."\n")
  texFile:write("  \\label{tab:".. testName .."-"..modelName.."}\n")
  texFile:write("  \\begin{tabular}{ |"..columns.."}".."\n")
  texFile:write("    \\hline".."\n")
  --Header
  texFile:write("    Qualidade")
  for i, pose in ipairs(logFileTable.poses) do
    texFile:write(" & Pose " .. i)
  end
  texFile:write("    \\\\ \n")
  texFile:write("    \\hline".."\n")
  
  --Content
  for i, parameterLine in ipairs(parameterTableLines) do
    texFile:write("    " .. parameterLine .. " \\\\ \n")
    texFile:write("    \\hline".."\n")
  end
  
  texFile:write("  \\end{tabular}".."\n")
  texFile:write("\\end{table}".."\n")
  
  
  -- Move Figures
  for i, logFileTable in ipairs(logFileTableList) do
    for j, pose in ipairs(logFileTable[logFileTable.algorithms[1].name]) do
      local cmdLine = string.gsub("copy \"" .. workingDir .. pose.screenshotTest.screenshotPath .. pose.screenshotTest.screenshotFile .. "\"  "   
                                  .. workingDir .. logFilePath .. analiseLogDir .. "/" .. figuresDir .. "/" .. pose.screenshotTest.screenshotFile, "/", "\\")
      os.execute(cmdLine)
    end
  end
  

  texFile:write("\\newpage\n")
  -- SubFloats 
  texFile:write("\\begin{figure}[H]\n")
  texFile:write("  \\centering\n")
  for i, logFileTable in ipairs(logFileTableList) do
    local alg = logFileTable.algorithms[1]
    for j, pose in ipairs(logFileTable[alg.name]) do
      local n = #logFileTable[alg.name]
      local size = math.min(math.floor(.5 + 100/n)/100, .5)
      texFile:write("  \\subfloat{\\includegraphics[width=".. tostring(size) .."\\textwidth]{".. figuresDir .. "/" .. pose.screenshotTest.screenshotFile .. "} }" .. (j == n and "\\\\" or "") .. "\n")
    end
    texFile:write("  " .. nameLines[i] .. "\\\\ \n")
  end

  for j, pose in ipairs(logFileTable.poses) do
    local n = #logFileTable.poses
    local size = math.min(math.floor(.5 + 100/n)/100, .5)
    texFile:write("  \\subfloat{\\includegraphics[width=".. tostring(size) .."\\textwidth]{".. figuresDir .. "/" .. modelName .. pose .. ".png" .. "} }" .. (j == n and "\\\\" or "") .. "\n")
  end
  texFile:write("  Optix \\\\ \n")

  texFile:write("  \\caption{Comparação entre diferentes configurações de qualidade do modelo "..modelName..".}\n")
  texFile:write("  \\label{fig:".. testName .."-"..modelName.."}\n")
  texFile:write("\\end{figure}\n")
  texFile:write("\n")
  texFile:write("\n")
  
  
  texFile:close()
  
end

function diffuseScreenshotQualityLogAnaliser(logFileNameList)
  local testName = "DiffuseScreenshotQuality"
  local timeMetric = "Mean" -- {"Mean" | "Single"}
  
  local logFileList = {}
  for i, logFileName in ipairs(logFileNameList) do
    local logFile = loadfile(workingDir .. logFilePath .. logFileName)
    table.insert(logFileList, logFile)
  end
  
  local logFileTableList = {}
  for i, logFile in ipairs(logFileList) do
    local logFileTable = logFile()
    table.insert(logFileTableList, logFileTable)
  end  
  
  local logFileTable = logFileTableList[1]
  local modelName = logFileTable.scene.models[1][1]
    
  -- Create Result Log Dir
  local analiseLogDir = testName .. "_" .. modelName .. "_analysis"
  local cmdStr = string.gsub("mkdir " .. workingDir .. logFilePath .. analiseLogDir, "/", "\\")
  os.execute(cmdStr)
    
  -- Create Result Log Figure Dir
  local figuresDir = "figures"
  local cmdStr = string.gsub("mkdir " .. workingDir .. logFilePath .. analiseLogDir .. "/" .. figuresDir, "/", "\\")
  os.execute(cmdStr)
    
  -- Create Result Log File
  local texFileName = testName .. "_" .. modelName .. "_analysis.tex"
  local texFile = io.open(workingDir .. logFilePath .. analiseLogDir .. "/" .. texFileName, "w")

  texFile:write("\\subsection{Modelo ".. modelName .."}\n\n")
  --Intro
  texFile:write("%Tabela de comparação qualidade com iluminação difusa.\n\n")
  texFile:write("%Gerada em " .. logFileTable.creation_date .. "\n\n")
  texFile:write("\n")
  
  --Algorithms
  for i, alg in ipairs(logFileTable.algorithms) do
    texFile:write("%Algoritmo utilizado: " .. alg.name .. "\n\n")
  end
  texFile:write("\n")
    
  --Scene
  texFile:write("%A cena é composta pelo modelo ".. modelName ..".".."\n\n")
  texFile:write("%A resolução utilizada é ".. logFileTable.resolution[1] .."x" .. logFileTable.resolution[2].."\n\n")
  texFile:write("A cena utilizada neste teste é composta pelo modelo \\emph{" .. modelName .. "}, repetido 4 vezes, e possui " .. math.floor(logFileTable.scene.totalVertices/1000) .. " mil vértices e ".. math.floor(logFileTable.scene.totalTriangles/1000)  .. " mil triângulos. ")
  -- texFile:write("A Tabela \\ref{tab:".. testName .."-" .. modelName .. "} contém os resultados de tempo para a execução de cada uma das configurações. ")
  texFile:write("As imagens na Figura \\ref{fig:".. testName .."-"..modelName.."} visam apresentar uma comparação entre a iluminação difusa e oclusão ambiente.\n\n")
  texFile:write("\n")
  
  
  -- Move Figures
  for i, logFileTable in ipairs(logFileTableList) do
    for j, pose in ipairs(logFileTable[logFileTable.algorithms[1].name]) do
      local cmdLine = string.gsub("copy \"" .. workingDir .. pose.screenshotTest.screenshotPath .. pose.screenshotTest.screenshotFile .. "\"  "   
                                  .. workingDir .. logFilePath .. analiseLogDir .. "/" .. figuresDir .. "/" .. pose.screenshotTest.screenshotFile, "/", "\\")
      os.execute(cmdLine)
    end
  end
  
  -- local nameLines = {"Iluminação difusa desligada e oclusão ambiente ligada", "Iluminação difusa ligada e oclusão ambiente ligada", "Iluminação difusa ligada e oclusão ambiente desligada",}
  local nameLines = {"Iluminação difusa:Desligada | Oclusão ambiente: Ligada", "Iluminação difusa:Ligada | Oclusão ambiente: Ligada", "Iluminação difusa:Ligada | Oclusão ambiente: Desligada"}
  texFile:write("\\newpage\n")
  -- SubFloats 
  texFile:write("\\begin{figure}[H]\n")
  texFile:write("  \\centering\n")
  for i, logFileTable in ipairs(logFileTableList) do
    local alg = logFileTable.algorithms[1]
    for j, pose in ipairs(logFileTable[alg.name]) do
      local n = #logFileTable[alg.name]
      local size = math.min(math.floor(.5 + 100/n)/100, .5)
      texFile:write("  \\subfloat{\\includegraphics[width=".. tostring(size) .."\\textwidth]{".. figuresDir .. "/" .. pose.screenshotTest.screenshotFile .. "} }" .. (j == n and "\\\\" or "") .. "\n")
    end
    texFile:write("  " .. nameLines[i] .. "\\\\ \n")
  end

  texFile:write("  \\caption{Comparação entre utilização de oclusão ambiente e iluminação difusa para o modelo \\emph{"..modelName.."}.}\n")
  texFile:write("  \\label{fig:".. testName .."-"..modelName.."}\n")
  texFile:write("\\end{figure}\n")
  texFile:write("\n")
  texFile:write("\n")
  
  
  texFile:close()
  
end

function timeTableLogAnaliser(logFileName)
  local testName = "TimeTable"
  local timeMetric = "Mean" -- {"Mean" | "Single"}
  local logFile = loadfile(workingDir .. logFilePath .. logFileName)
  if logFile then
    local logFileTable = logFile()
    local modelName = logFileTable.scene.models[1][1]
    
    -- Create Result Log Dir
    local analiseLogDir = testName .. "_" .. modelName .. "_analysis"
    local cmdStr = string.gsub("mkdir " .. workingDir .. logFilePath .. analiseLogDir, "/", "\\")
    os.execute(cmdStr)
    
    -- Create Result Log File
    local texFileName = testName .. "_analysis.tex"
    local texFile = io.open(workingDir .. logFilePath .. analiseLogDir .. "/" .. texFileName, "w")
    
    --Intro
    texFile:write("%Tabela de comparação de tempo por etapas.\n\n")
    texFile:write("%Gerada em " .. logFileTable.creation_date .. "\n\n")
    texFile:write("\n")
    --Algorithms
    for i, alg in ipairs(logFileTable.algorithms) do
      texFile:write("%Algoritmo utilizado: " .. alg.name .. "\n\n")
      texFile:write("%Os parâmetros de execução são: \n")
      texFile:write("%\\begin{itemize}\n")
      for j, parm in ipairs(alg) do
        texFile:write("%  \\item " .. tostring(parm[1]) .. " = " .. tostring(parm[2]) .. "\n")
      end
      texFile:write("%\\end{itemize}\n")
    end
    texFile:write("\n")
    
    --Scene
    texFile:write("%A cena é composta pelo modelo ".. modelName ..".".."\n\n")
    texFile:write("A cena utilizada neste teste é composta pelo modelo \\emph{" .. modelName .. "} e possui " .. math.floor(logFileTable.scene.totalVertices/1000) .. " mil vértices e ".. math.floor(logFileTable.scene.totalTriangles/1000)  .. " mil triângulos.\n\n")
    texFile:write("%A resolução utilizada é ".. logFileTable.resolution[1] .."x" .. logFileTable.resolution[2].."\n\n")
    texFile:write("\n")
    
    --TableLines
    local tableLines = {}
    local pose = logFileTable[logFileTable.algorithms[1].name][1]
    local totalMean = 0
    for i, step in ipairs(pose.timeTest.steps) do
      totalMean = totalMean + pose.timeTest.steps[i].GPU[timeMetric]
    end
    
    for i, step in ipairs(pose.timeTest.steps) do
      local tableLine = step.name
      local ms = math.floor(pose.timeTest.steps[i].GPU[timeMetric]*100+.5)/100
      -- local fps = math.floor((1.0/(ms/1000.0))*100 + .5)/100
      -- local totalMean = pose.timeTest.total.GPU[timeMetric]
      local percent = math.floor((1000*pose.timeTest.steps[i].GPU[timeMetric]/totalMean)+.5)/10
      tableLine = tableLine .. " & " .. tostring(ms) 
      -- tableLine = tableLine .. " & " .. tostring(fps) 
      tableLine = tableLine .. " & " .. tostring(percent) .. " \\%"
      table.insert(tableLines, tableLine)
    end
    local tableLine = "Total"
    local ms = math.floor(totalMean*100+.5)/100
    -- local fps = math.floor((1.0/(ms/1000.0))*100 + .5)/100
    tableLine = tableLine .. " & " .. tostring(ms)
    -- tableLine = tableLine .. " & " .. tostring(fps) 
    tableLine = tableLine .. " & --"
    table.insert(tableLines, tableLine)
    
    
    -- Table
    local columns = " c | c | c |"

    
    texFile:write("%A tabela a seguir apresenta os resultados obtidos para cada etapa." .. "\n")
    texFile:write("\\begin{table}[H]".."\n")
    texFile:write("  \\centering".."\n")
    texFile:write("  \\caption{Resultados de tempo para a execução de cada uma das etapas.}".."\n")
    texFile:write("  \\label{tab:".. testName .."}\n")
    texFile:write("  \\begin{tabular}{ |"..columns.."}".."\n")
    texFile:write("    \\hline".."\n")
    --Header
    texFile:write("   Etapa & Tempo (ms) & Percentual")
    
    texFile:write("  \\\\ \n")
    texFile:write("    \\hline".."\n")
    
    --Content
    for i, tabLine in ipairs(tableLines) do
      texFile:write("    " .. tabLine .. " \\\\ \n")
      texFile:write("    \\hline".."\n")
    end
    
    texFile:write("  \\end{tabular}".."\n")
    texFile:write("\\end{table}".."\n")
    texFile:close()
  end
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
if activeTests.timeTable then  
  timeTableTest()
end

if activeTests.parameterVariation  then  
  parameterVariationTest()
end  

if activeTests.screenshotQuality  then  
  screenshotQualityTest()
end

if activeTests.diffuseScreenshotQuality  then  
  diffuseScreenshotQualityTest()
end

if activeTests.geometryScalability  then  
  geometryScalabilityTest()
end

if activeTests.scalability  then  
  scalabilityTest()
end

if activeTests.algorithmCompare then  
  algorithmCompareTest()
end








$ErrorActionPreference = "Stop"

# region 项目参数

$project_name = "stm32h743-project"
$cmake_config = "arm-none-eabi-cortex-m7-release"
$platform = "arm-none-eabi-cortex-m7"
$workspace_path = Get-Location
$build_path = "$workspace_path/jc_build"
$install_path = "$env:cpp_lib_build_scripts_path/${platform}/.libs/${project_name}"

# endregion

# region 如果还没编译安装，就先编译安装。

New-Item -Path $build_path -ItemType Directory -Force
Push-Location $build_path
try
{
	cmake -G "Ninja" $workspace_path `
		--preset "$cmake_config"

	if ($LASTEXITCODE)
	{
		throw "配置失败"
	}

	ninja -j12
	if ($LASTEXITCODE)
	{
		throw "编译失败"
	}

	ninja install
}
finally
{
	Pop-Location
}

# endregion

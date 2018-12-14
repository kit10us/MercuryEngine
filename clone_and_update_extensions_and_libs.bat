
echo "Pulling extensions and libraries..."

echo "   Pulling unify..."
mkdir ..\unify
git clone https://github.com/kit10us/unify.git ../unify
git pull ../unify

echo    Pulling MEASELoader...
mkdir ..\MEASELoader
git clone https://github.com/kit10us/MEASELoader.git ../MEASELoader
git pull ../MEASELoader

echo "   Pulling MEDaeLoader..."
mkdir ..\MEDaeLoader
git clone https://github.com/kit10us/MEDaeLoader.git ../MEDaeLoader
git pull ../MEDaeLoader

echo "   Pulling MESceneAutoLoader..."
mkdir ..\MESceneAutoLoader
git clone https://github.com/kit10us/MESceneAutoLoader.git ../MESceneAutoLoader
git pull ../MESceneAutoLoader

echo    Pulling qjson..."
mkdir ..\qjson
git clone https://github.com/kit10us/qjson.git ../qjson
git pull ../qjson

echo    Pulling qxml...
mkdir ..\qxml
git clone https://github.com/kit10us/qxml.git ../qxml
git pull ../qxml

echo    Pulling packages...
mkdir  ..\packages
git clone https://github.com/kit10us/sas_packages.git ../packages
git pull ../packages


echo Done getting extensions and libraries.

pause
#ifndef _DEPTH_MAP_FILE_IO_
#define _DEPTH_MAP_FILE_IO_
#include <vector>
#include <fstream>
template <class DataType>
class DepthmapFileIOBase
{
public:
	DepthmapFileIOBase();
	void SetVersion(int v){ Version  = v; }
	void SetDepthmapType(int t) {DepthmapType = t;}
	void SetWidthHeight(int w, int h) {Width = w; Height = h;}
	void SetData(std::vector<DataType> & data);

	void SaveTofile(char* file_name);
	void LoadFromfile(char* file_name);
protected:
	
	int Version;
	//深度图类型,暂时无用
	int DepthmapType;
	char DataTypeName[8];
	int Width, Height;
	std::vector<DataType> data;
};




//////////////////////////////////////////////////////////////////////////

template <class DataType>
class DepthmapFileIO
{
private:
	DepthmapFileIO(){}
};
template<>
class DepthmapFileIO<float> : public DepthmapFileIOBase<float>
{
public:
	DepthmapFileIO()
	{
		memset(DataTypeName, 0, 8);
		strcpy(DataTypeName, "float");
	};
};
template<>
class DepthmapFileIO<double>: public DepthmapFileIOBase<double>
{
public:
	DepthmapFileIO()
	{
		memset(DataTypeName, 0, 8);
		strcpy(DataTypeName, "double");
	};
};




//////////////////////////////////////////////////////////////////////////
template <class DataType>
DepthmapFileIOBase<DataType>::DepthmapFileIOBase()
{
	Version = 1;
	DepthmapType = 0;
}

template <class DataType>
void DepthmapFileIOBase<DataType>::SaveTofile(char* file_name)
{
	std::ofstream out(file_name,  std::ios::binary);
	
	if (!out.is_open())
	{
		std::cout << "File not open, cannot save.\n";
		exit(0);
	}
	out.write((char const*)&Version, 4);
	out.write((char const*)&DepthmapType, 4);
	out.write((char const*)DataTypeName, 8);
	out.write((char const*)&Width, 4);out.write((char const*)&Height, 4);

	DataType d = 0.0;
	for (int i = 0; i < data.size(); i++)
	{
		d = data[i];
		out.write((char const*)&d, sizeof(DataType));
	}

	out.close();
}


template <class DataType>
void DepthmapFileIOBase<DataType>::LoadFromfile(char* file_name)
{
	std::ifstream in(file_name, std::ios::binary);
	if (!in.is_open())
	{
		std::cout << "File not open, cannot laod.\n";
		exit(0);
	}

	in.read((char *)&Version, 4);
	in.read((char *)&DepthmapType, 4);
	in.read((char *)DataTypeName, 8);
	if (0==strcmp(DataTypeName, "float"))
	{
		if (typeid(double).hash_code() == typeid(DataType).hash_code())
		{
			std::cout << "Warning! File encode with float, but read and store as double in memory, nothing wrong but wasting.\n";
		}
	}
	else if (0==strcmp(DataTypeName, "double"))
	{
		if (typeid(float).hash_code() == typeid(DataType).hash_code())
		{
			std::cout << "Warning! File encode with double, but read as double, May lose precision.\n";
		}
	}
	else
	{
		std::cout << "Unsupported data type, float and double only!\n";
		exit(0);
	}
	in.read((char *)&Width, 4);in.read((char *)&Height, 4);

	int size = Width * Height;
	int data_unit_size = (strcmp(DataTypeName, "float")==0) ? 4 : 8;

	DataType d = 0.0;
	for (int i = 0; i < size; i++)
	{
		in.read((char *)&d, data_unit_size);
		data.push_back(d);
	}

	in.close();
}


template <class DataType>
void DepthmapFileIOBase<DataType>::SetData(std::vector<DataType> & _data)
{
	if (_data.size() != Width*Height)
	{
		std::cout << "Data size not match!\n";
		exit(0);
	}
	data = _data;
}


#endif
#include "Math/SupportMethods.h"

void SupportMethods::PrintMass(float** Mass, int Size)
{
	for (int i = 0; i < Size; i++)
	{
		FString str=FString();
		for (int j = 0; j < Size; j++)
		{
			str+=FString::Printf(TEXT("%f"),Mass[i][j]);
			str+=" ";
		}
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red,str);
	}

}

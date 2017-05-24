/*============================================================================*/
/*                        DIAdem Connectivity Library                         */
/*----------------------------------------------------------------------------*/
/*    Copyright (c) National Instruments 2004-2009.  All Rights Reserved.     */
/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Title:       nilibddc.h                                                    */
/* Purpose:     Include file for DIAdem Connectivity Library                  */
/*                                                                            */
/*============================================================================*/

#ifndef _NILIBDDC_H
#define _NILIBDDC_H

#include <stdarg.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

//*****************************************************************************
// Typedefs, enums, constants, etc.
//*****************************************************************************

#ifdef WIN32
	#pragma pack(push)
	#pragma pack(4)
#endif

typedef struct _DDCFile				DDCFile;
typedef struct _DDCChannelGroup		DDCChannelGroup;
typedef struct _DDCChannel			DDCChannel;

typedef DDCFile*					DDCFileHandle;
typedef DDCChannelGroup*			DDCChannelGroupHandle;
typedef DDCChannel*					DDCChannelHandle;

#ifdef WIN32
	#pragma pack(pop)
#endif

typedef enum {
	DDC_UInt8		= 5,	// unsigned char
	DDC_Int16		= 2,	// short
	DDC_Int32		= 3,	// int
	DDC_Float		= 9,	// float
	DDC_Double		= 10,	// double
	DDC_String		= 23,	// string
	DDC_Timestamp	= 30,	// timestamp (Year/Month/Day/Hour/Minute/Second/Millisecond components)
} DDCDataType;

// File type constants
#define DDC_FILE_TYPE_TDM				"TDM"
#define DDC_FILE_TYPE_TDM_STREAMING		"TDMS"

// File property constants
#define DDC_FILE_NAME					"name"				// Name
#define DDC_FILE_DESCRIPTION			"description"		// Description
#define DDC_FILE_TITLE					"title"				// Title
#define DDC_FILE_AUTHOR					"author"			// Author
#define DDC_FILE_DATETIME				"datetime"			// Date/Time

// ChannelGroup property constants
#define DDC_CHANNELGROUP_NAME			"name"				// Name
#define DDC_CHANNELGROUP_DESCRIPTION	"description"		// Description

// Channel property constants
#define DDC_CHANNEL_NAME				"name"				// Name
#define DDC_CHANNEL_DESCRIPTION			"description"		// Description
#define DDC_CHANNEL_UNIT_STRING			"unit_string"		// Unit String
#define DDC_CHANNEL_MINIMUM				"minimum"			// Minimum
#define DDC_CHANNEL_MAXIMUM				"maximum"			// Maximum

// Error codes
typedef enum {

	DDC_NoError									= 0,			// No error
	DDC_ErrorBegin								= -6201,		//

	DDC_OutOfMemory								= -6201,		// The library could not allocate memory.
	DDC_InvalidArgument							= -6202,		// An invalid argument was passed to the library.
	DDC_InvalidDataType							= -6203,		// An invalid data type was passed to the library.
	DDC_UnexpectedError							= -6204,		// An unexpected error occurred in the library.
	DDC_UsiCouldNotBeLoaded						= -6205,		// The USI engine could not be loaded.
	DDC_InvalidFileHandle						= -6206,		// An invalid file handle was passed to the library.
	DDC_InvalidChannelGroupHandle				= -6207,		// An invalid channel group handle was passed to the library.
	DDC_InvalidChannelHandle					= -6208,		// An invalid channel handle was passed to the library.
	DDC_FileDoesNotExist						= -6209,		// The file passed to the library does not exist.
	DDC_CannotWriteToReadOnlyFile				= -6210,		// The file passed to the library is read only and cannot be modified.
	DDC_StorageCouldNotBeOpened					= -6211,		// The storage could not be opened.
	DDC_FileAlreadyExists						= -6212,		// The file passed to the library already exists and cannot be created.
	DDC_PropertyDoesNotExist					= -6213,		// The property passed to the library does not exist.
	DDC_PropertyDoesNotContainData				= -6214,		// The property passed to the library does not have a value.
	DDC_PropertyIsNotAScalar					= -6215,		// The value of the property passed to the library is an array and not a scalar.
	DDC_DataObjectTypeNotFound					= -6216,		// The object type passed to the library does not exist.
	DDC_NotImplemented							= -6217,		// The current implementation does not support this operation.
	DDC_CouldNotSaveFile						= -6218,		// The file could not be saved.
	DDC_MaximumNumberOfDataValuesExceeded		= -6219,		// The request would exceed the maximum number of data values for a channel.
	DDC_InvalidChannelName						= -6220,		// An invalid channel name was passed to the library.
	DDC_DuplicateChannelName					= -6221,		// The channel group already contains a channel with this name.
	DDC_DataTypeNotSupported					= -6222,		// The current implementation does not support this data type.
	DDC_FileAccessDenied						= -6224,		// File access denied.
	DDC_InvalidTimeValue						= -6225,		// The specified time value is invalid.
	DDC_ReplaceNotSupportedForSavedTDMSData		= -6226,		// The replace operation is not supported on data that has already been saved to a TDM Streaming file.
	DDC_PropertyDataTypeMismatch				= -6227,		// The data type of the property does not match the expected data type.
	DDC_ChannelDataTypeMismatch					= -6228,		// The data type of the channel does not match the expected data type.

    DDC_ErrorEnd                                = -6228,        //
	DDC_ErrorForceSizeTo32Bits					= 0xffffffff	//

} DDCError;

//*****************************************************************************
/// -> Object Management
//*****************************************************************************
int __stdcall DDC_CreateFile (const char *filePath,
							  const char *fileType,
							  const char *name,
							  const char *description,
							  const char *title,
							  const char *author,
							  DDCFileHandle *file);

int __stdcall DDC_AddChannelGroup (DDCFileHandle file,
								   const char *name,
								   const char *description,
								   DDCChannelGroupHandle *channelGroup);

int __stdcall DDC_AddChannel (DDCChannelGroupHandle channelGroup,
							  DDCDataType dataType,
							  const char *name,
							  const char *description,
							  const char *unitString,
							  DDCChannelHandle *channel);

int __stdcall DDC_SaveFile (DDCFileHandle file);

int __stdcall DDC_CloseFile (DDCFileHandle file);

int __stdcall DDC_OpenFileEx (const char *filePath,
							  const char *fileType,
							  int readOnly,
							  DDCFileHandle *file);

//*****************************************************************************
/// -> Advanced
//*****************************************************************************
int __stdcall DDC_RemoveChannelGroup (DDCChannelGroupHandle channelGroup);

int __stdcall DDC_RemoveChannel (DDCChannelHandle channel);

int __stdcall DDC_CloseChannelGroup (DDCChannelGroupHandle channelGroup);

int __stdcall DDC_CloseChannel (DDCChannelHandle channel);

//*****************************************************************************
/// <- Advanced
//*****************************************************************************
//*****************************************************************************
/// -> Obsolete
//*****************************************************************************
int __stdcall DDC_OpenFile (const char *filePath,
							const char *fileType,
							DDCFileHandle *file);

//*****************************************************************************
/// <- Obsolete
//*****************************************************************************
//*****************************************************************************
/// <- Object Management
//*****************************************************************************

//*****************************************************************************
/// -> Data Storage
//*****************************************************************************
int __stdcall DDC_SetDataValues (DDCChannelHandle channel,
								 void *values,
								 size_t numValues);

int __stdcall DDC_SetDataValuesTimestampComponents (DDCChannelHandle channel,
													unsigned int year[], 
													unsigned int month[], 
													unsigned int day[],
													unsigned int hour[], 
													unsigned int minute[], 
													unsigned int second[],
													double milliSecond[],
													size_t numValues);

int __stdcall DDC_AppendDataValues (DDCChannelHandle channel,
									void *values,
									size_t numValues);

int __stdcall DDC_AppendDataValuesTimestampComponents (DDCChannelHandle channel,
													   unsigned int year[], 
													   unsigned int month[], 
													   unsigned int day[],
													   unsigned int hour[], 
													   unsigned int minute[], 
													   unsigned int second[],
													   double milliSecond[],
													   size_t numValues);

int __stdcall DDC_ReplaceDataValues (DDCChannelHandle channel,
									 size_t indexOfFirstValueToReplace,
									 void *values,
									 size_t numValues);

int __stdcall DDC_ReplaceDataValuesTimestampComponents (DDCChannelHandle channel,
														size_t indexOfFirstValueToReplace,
														unsigned int year[], 
														unsigned int month[], 
														unsigned int day[],
														unsigned int hour[], 
														unsigned int minute[], 
														unsigned int second[],
														double milliSecond[],
														size_t numValues);

//*****************************************************************************
/// <- Data Storage
//*****************************************************************************

//*****************************************************************************
/// -> Data Retrieval
//*****************************************************************************

//*****************************************************************************
/// -> Enumeration
//*****************************************************************************
int __stdcall DDC_GetNumChannelGroups (DDCFileHandle file,
									   unsigned int *numChannelGroups);

int __stdcall DDC_GetChannelGroups (DDCFileHandle file,
									DDCChannelGroupHandle channelGroupsBuf[],
									size_t numChannelGroups);

int __stdcall DDC_GetNumChannels (DDCChannelGroupHandle channelGroup,
								  unsigned int *numChannels);

int __stdcall DDC_GetChannels (DDCChannelGroupHandle channelGroup,
							   DDCChannelHandle channelsBuf[],
							   size_t numChannels);

//*****************************************************************************
/// <- Enumeration
//*****************************************************************************

int __stdcall DDC_GetNumDataValues (DDCChannelHandle channel,
									unsigned __int64 *numValues);

int __stdcall DDC_GetDataValues (DDCChannelHandle channel,
								 size_t indexOfFirstValueToGet,
								 size_t numValuesToGet,
								 void *values);

int __stdcall DDC_GetDataValuesTimestampComponents (DDCChannelHandle channel,
													size_t indexOfFirstValueToGet,
													size_t numValuesToGet,
													unsigned int year[], 
													unsigned int month[], 
													unsigned int day[],
													unsigned int hour[], 
													unsigned int minute[], 
													unsigned int second[],
													double milliSecond[],
													unsigned int weekDay[]);

int __stdcall DDC_GetDataType (DDCChannelHandle channel,
							   DDCDataType *dataType);

//*****************************************************************************
/// <- Data Retrieval
//*****************************************************************************

//*****************************************************************************
/// -> Properties
//*****************************************************************************

//*****************************************************************************
/// -> File
//*****************************************************************************
int __cdecl DDC_SetFileProperty (DDCFileHandle file,
								 const char *property,
								 ...);

int __stdcall DDC_SetFilePropertyTimestampComponents (DDCFileHandle file,
													  const char *property,
													  unsigned int year, 
													  unsigned int month, 
													  unsigned int day,
													  unsigned int hour, 
													  unsigned int minute, 
													  unsigned int second, 
													  double milliSecond);

int __stdcall DDC_SetFilePropertyV (DDCFileHandle file,
									const char *property,
									va_list args);

int __stdcall DDC_GetFileProperty (DDCFileHandle file,
								   const char *property,
								   void *value,
								   size_t valueSizeInBytes);

int __stdcall DDC_GetFilePropertyTimestampComponents (DDCFileHandle file,
													  const char *property,
													  unsigned int *year, 
													  unsigned int *month, 
													  unsigned int *day,
													  unsigned int *hour, 
													  unsigned int *minute, 
													  unsigned int *second, 
													  double *milliSecond,
													  unsigned int *weekDay);

int __stdcall DDC_GetFileStringPropertyLength (DDCFileHandle file,
											   const char *property,
											   unsigned int *length);

int __cdecl DDC_CreateFileProperty (DDCFileHandle file,
									const char *property,
									DDCDataType dataType,
									...);

int __stdcall DDC_CreateFilePropertyTimestampComponents (DDCFileHandle file,
														 const char *property,
														 unsigned int year, 
														 unsigned int month, 
														 unsigned int day,
														 unsigned int hour, 
														 unsigned int minute, 
														 unsigned int second, 
														 double milliSecond);

int __stdcall DDC_CreateFilePropertyV (DDCFileHandle file,
									   const char *property,
									   DDCDataType dataType,
									   va_list args);

int __stdcall DDC_FilePropertyExists (DDCFileHandle file,
									  const char *property,
									  int *exists);

int __stdcall DDC_GetNumFileProperties (DDCFileHandle file, 
										unsigned int *numProperties);

int __stdcall DDC_GetFilePropertyNames (DDCFileHandle file, 
										char** propertyNames, 
										size_t numPropertyNames);

int __stdcall DDC_GetFilePropertyType (DDCFileHandle file, 
									   const char *property, 
									   DDCDataType *dataType);

//*****************************************************************************
/// <- File
//*****************************************************************************

//*****************************************************************************
/// -> Channel Group
//*****************************************************************************
int __cdecl DDC_SetChannelGroupProperty (DDCChannelGroupHandle channelGroup,
										 const char *property,
										 ...);

int __stdcall DDC_SetChannelGroupPropertyTimestampComponents (DDCChannelGroupHandle channelGroup,
															  const char *property,
															  unsigned int year, 
															  unsigned int month, 
															  unsigned int day,
															  unsigned int hour, 
															  unsigned int minute, 
															  unsigned int second, 
															  double milliSecond);

int __stdcall DDC_SetChannelGroupPropertyV (DDCChannelGroupHandle channelGroup,
											const char *property,
											va_list args);

int __stdcall DDC_GetChannelGroupProperty (DDCChannelGroupHandle channelGroup,
										   const char *property,
										   void *value,
										   size_t valueSizeInBytes);

int __stdcall DDC_GetChannelGroupPropertyTimestampComponents (DDCChannelGroupHandle channelGroup,
															  const char *property,
															  unsigned int *year, 
															  unsigned int *month, 
															  unsigned int *day,
															  unsigned int *hour, 
															  unsigned int *minute, 
															  unsigned int *second, 
															  double *milliSecond,
															  unsigned int *weekDay);

int __stdcall DDC_GetChannelGroupStringPropertyLength (DDCChannelGroupHandle channelGroup,
													   const char *property,
													   unsigned int *length);

int __cdecl DDC_CreateChannelGroupProperty (DDCChannelGroupHandle channelGroup,
											const char *property,
											DDCDataType dataType,
											...);

int __stdcall DDC_CreateChannelGroupPropertyTimestampComponents (DDCChannelGroupHandle channelGroup,
																 const char *property,
																 unsigned int year, 
																 unsigned int month, 
																 unsigned int day,
																 unsigned int hour, 
																 unsigned int minute, 
																 unsigned int second, 
																 double milliSecond);

int __stdcall DDC_CreateChannelGroupPropertyV (DDCChannelGroupHandle channelGroup,
											   const char *property,
											   DDCDataType dataType,
											   va_list args);

int __stdcall DDC_ChannelGroupPropertyExists (DDCChannelGroupHandle channelGroup,
											  const char *property,
											  int *exists);

int __stdcall DDC_GetNumChannelGroupProperties (DDCChannelGroupHandle channelGroup, 
												unsigned int *numProperties);

int __stdcall DDC_GetChannelGroupPropertyNames (DDCChannelGroupHandle channelGroup, 
												char** propertyNames, 
												size_t numPropertyNames);

int __stdcall DDC_GetChannelGroupPropertyType (DDCChannelGroupHandle channelGroup, 
											   const char *property, 
											   DDCDataType *dataType);

//*****************************************************************************
/// <- Channel Group
//*****************************************************************************

//*****************************************************************************
/// -> Channel
//*****************************************************************************
int __cdecl DDC_SetChannelProperty (DDCChannelHandle channel,
									const char *property,
									...);

int __stdcall DDC_SetChannelPropertyTimestampComponents (DDCChannelHandle channel,
														 const char *property,
														 unsigned int year, 
														 unsigned int month, 
														 unsigned int day,
														 unsigned int hour, 
														 unsigned int minute, 
														 unsigned int second, 
														 double milliSecond);

int __stdcall DDC_SetChannelPropertyV (DDCChannelHandle channel,
									   const char *property,
									   va_list args);

int __stdcall DDC_GetChannelProperty (DDCChannelHandle channel,
									  const char *property,
									  void *value,
									  size_t valueSizeInBytes);

int __stdcall DDC_GetChannelPropertyTimestampComponents (DDCChannelHandle channel,
														 const char *property,
														 unsigned int *year, 
														 unsigned int *month, 
														 unsigned int *day,
														 unsigned int *hour, 
														 unsigned int *minute, 
														 unsigned int *second, 
														 double *milliSecond,
														 unsigned int *weekDay);

int __stdcall DDC_GetChannelStringPropertyLength (DDCChannelHandle channel,
												  const char *property,
												  unsigned int *length);

int __cdecl DDC_CreateChannelProperty (DDCChannelHandle channel,
									   const char *property,
									   DDCDataType dataType,
									   ...);

int __stdcall DDC_CreateChannelPropertyTimestampComponents (DDCChannelHandle channel,
															const char *property,
															unsigned int year, 
															unsigned int month, 
															unsigned int day,
															unsigned int hour, 
															unsigned int minute, 
															unsigned int second, 
															double milliSecond);

int __stdcall DDC_CreateChannelPropertyV (DDCChannelHandle channel,
										  const char *property,
										  DDCDataType dataType,
										  va_list args);

int __stdcall DDC_ChannelPropertyExists (DDCChannelHandle channel,
										 const char *property,
										 int *exists);

int __stdcall DDC_GetNumChannelProperties (DDCChannelHandle channel, 
										   unsigned int *numProperties);

int __stdcall DDC_GetChannelPropertyNames (DDCChannelHandle channel, 
										   char** propertyNames, 
										   size_t numPropertyNames);

int __stdcall DDC_GetChannelPropertyType (DDCChannelHandle channel, 
										  const char *property, 
										  DDCDataType *dataType);

//*****************************************************************************
/// <- Channel
//*****************************************************************************

//*****************************************************************************
/// <- Properties
//*****************************************************************************

//*****************************************************************************
/// -> Miscellaneous
//*****************************************************************************
const char * __stdcall DDC_GetLibraryErrorDescription (int errorCode);

void __stdcall DDC_FreeMemory (void *memoryPointer);

//*****************************************************************************
/// <- Miscellaneous
//*****************************************************************************

//*****************************************************************************
/// -> Separate type-safe functions for non-C users
//*****************************************************************************
int __stdcall DDC_SetDataValuesUInt8 (DDCChannelHandle channel, 
									  unsigned char values[], 
									  size_t numValues);

int __stdcall DDC_SetDataValuesInt16 (DDCChannelHandle channel, 
									  short values[], 
									  size_t numValues);

int __stdcall DDC_SetDataValuesInt32 (DDCChannelHandle channel, 
									  long values[], 
									  size_t numValues);

int __stdcall DDC_SetDataValuesFloat (DDCChannelHandle channel, 
									  float values[], 
									  size_t numValues);

int __stdcall DDC_SetDataValuesDouble (DDCChannelHandle channel, 
									   double values[], 
									   size_t numValues);

int __stdcall DDC_SetDataValuesString (DDCChannelHandle channel, 
									   const char *values[], 
									   size_t numValues);

int __stdcall DDC_AppendDataValuesUInt8 (DDCChannelHandle channel, 
										 unsigned char values[], 
										 size_t numValues);

int __stdcall DDC_AppendDataValuesInt16 (DDCChannelHandle channel, 
										 short values[], 
										 size_t numValues);

int __stdcall DDC_AppendDataValuesInt32 (DDCChannelHandle channel, 
										 long values[], 
										 size_t numValues);

int __stdcall DDC_AppendDataValuesFloat (DDCChannelHandle channel, 
										 float values[], 
										 size_t numValues);

int __stdcall DDC_AppendDataValuesDouble (DDCChannelHandle channel, 
										  double values[], 
										  size_t numValues);

int __stdcall DDC_AppendDataValuesString (DDCChannelHandle channel, 
										  const char *values[], 
										  size_t numValues);

int __stdcall DDC_ReplaceDataValuesUInt8 (DDCChannelHandle channel, 
										  size_t indexOfFirstValueToReplace,
										  unsigned char values[], 
										  size_t numValues);

int __stdcall DDC_ReplaceDataValuesInt16 (DDCChannelHandle channel, 
										  size_t indexOfFirstValueToReplace,
										  short values[], 
										  size_t numValues);

int __stdcall DDC_ReplaceDataValuesInt32 (DDCChannelHandle channel, 
										  size_t indexOfFirstValueToReplace,
										  long values[], 
										  size_t numValues);

int __stdcall DDC_ReplaceDataValuesFloat (DDCChannelHandle channel, 
										  size_t indexOfFirstValueToReplace,
										  float values[], 
										  size_t numValues);

int __stdcall DDC_ReplaceDataValuesDouble (DDCChannelHandle channel, 
										   size_t indexOfFirstValueToReplace,
										   double values[], 
										   size_t numValues);

int __stdcall DDC_ReplaceDataValuesString (DDCChannelHandle channel, 
										   size_t indexOfFirstValueToReplace,
										   const char *values[], 
										   size_t numValues);

int __stdcall DDC_GetDataValuesUInt8 (DDCChannelHandle channel, 
									  size_t indexOfFirstValueToGet,
									  size_t numValuesToGet, 
									  unsigned char values[]);

int __stdcall DDC_GetDataValuesInt16 (DDCChannelHandle channel, 
									  size_t indexOfFirstValueToGet,
									  size_t numValuesToGet, 
									  short values[]);

int __stdcall DDC_GetDataValuesInt32 (DDCChannelHandle channel, 
									  size_t indexOfFirstValueToGet,
									  size_t numValuesToGet, 
									  long values[]);

int __stdcall DDC_GetDataValuesFloat (DDCChannelHandle channel, 
									  size_t indexOfFirstValueToGet,
									  size_t numValuesToGet, 
									  float values[]);

int __stdcall DDC_GetDataValuesDouble (DDCChannelHandle channel, 
									   size_t indexOfFirstValueToGet,
									   size_t numValuesToGet, 
									   double values[]);

int __stdcall DDC_GetDataValuesString (DDCChannelHandle channel, 
									   size_t indexOfFirstValueToGet,
									   size_t numValuesToGet, 
									   char *values[]);

int __stdcall DDC_CreateFilePropertyUInt8 (DDCFileHandle file, 
										   const char *property, 
										   unsigned char value);

int __stdcall DDC_CreateFilePropertyInt16 (DDCFileHandle file, 
										   const char *property,
										   short value);

int __stdcall DDC_CreateFilePropertyInt32 (DDCFileHandle file, 
										   const char *property,
										   long value);

int __stdcall DDC_CreateFilePropertyFloat (DDCFileHandle file, 
										   const char *property,
										   float value);

int __stdcall DDC_CreateFilePropertyDouble (DDCFileHandle file, 
											const char *property,
											double value);

int __stdcall DDC_CreateFilePropertyString (DDCFileHandle file, 
											const char *property,
											const char *value);

int __stdcall DDC_SetFilePropertyUInt8 (DDCFileHandle file, 
										const char* property, 
										unsigned char value);

int __stdcall DDC_SetFilePropertyInt16 (DDCFileHandle file, 
										const char* property, 
										short value);

int __stdcall DDC_SetFilePropertyInt32 (DDCFileHandle file, 
										const char* property, 
										long value);

int __stdcall DDC_SetFilePropertyFloat (DDCFileHandle file, 
										const char* property, 
										float value);

int __stdcall DDC_SetFilePropertyDouble (DDCFileHandle file, 
										 const char* property, 
										 double value);

int __stdcall DDC_SetFilePropertyString (DDCFileHandle file, 
										 const char* property, 
										 const char *value);

int __stdcall DDC_GetFilePropertyUInt8 (DDCFileHandle file, 
										const char* property, 
										unsigned char* value);

int __stdcall DDC_GetFilePropertyInt16 (DDCFileHandle file, 
										const char* property, 
										short* value);

int __stdcall DDC_GetFilePropertyInt32 (DDCFileHandle file, 
										const char* property, 
										long* value);

int __stdcall DDC_GetFilePropertyFloat (DDCFileHandle file, 
										const char* property, 
										float* value);

int __stdcall DDC_GetFilePropertyDouble (DDCFileHandle file, 
										 const char* property, 
										 double* value);

int __stdcall DDC_GetFilePropertyString (DDCFileHandle file, 
										 const char* property, 
										 char value[],
										 size_t valueSize);

int __stdcall DDC_CreateChannelGroupPropertyUInt8 (DDCChannelGroupHandle channelGroup, 
												   const char *property,
												   unsigned char value);

int __stdcall DDC_CreateChannelGroupPropertyInt16 (DDCChannelGroupHandle channelGroup, 
												   const char *property,
												   short value);

int __stdcall DDC_CreateChannelGroupPropertyInt32 (DDCChannelGroupHandle channelGroup, 
												   const char *property,
												   long value);

int __stdcall DDC_CreateChannelGroupPropertyFloat (DDCChannelGroupHandle channelGroup, 
												   const char *property,
												   float value);

int __stdcall DDC_CreateChannelGroupPropertyDouble (DDCChannelGroupHandle channelGroup, 
													const char *property,
													double value);

int __stdcall DDC_CreateChannelGroupPropertyString(DDCChannelGroupHandle channelGroup, 
												   const char *property,
												   const char *value);

int __stdcall DDC_SetChannelGroupPropertyUInt8 (DDCChannelGroupHandle channelGroup, 
												const char* property, 
												unsigned char value);

int __stdcall DDC_SetChannelGroupPropertyInt16 (DDCChannelGroupHandle channelGroup, 
												const char* property, 
												short value);

int __stdcall DDC_SetChannelGroupPropertyInt32 (DDCChannelGroupHandle channelGroup, 
												const char* property, 
												long value);

int __stdcall DDC_SetChannelGroupPropertyFloat (DDCChannelGroupHandle channelGroup, 
												const char* property, 
												float value);

int __stdcall DDC_SetChannelGroupPropertyDouble (DDCChannelGroupHandle channelGroup, 
												 const char* property, 
												 double value);

int __stdcall DDC_SetChannelGroupPropertyString (DDCChannelGroupHandle channelGroup, 
												 const char* property, 
												 const char *value);

int __stdcall DDC_GetChannelGroupPropertyUInt8 (DDCChannelGroupHandle channelGroup, 
												const char* property,
												unsigned char *value);

int __stdcall DDC_GetChannelGroupPropertyInt16 (DDCChannelGroupHandle channelGroup, 
												const char* property,
												short *value);

int __stdcall DDC_GetChannelGroupPropertyInt32 (DDCChannelGroupHandle channelGroup, 
												const char* property,
												long *value);

int __stdcall DDC_GetChannelGroupPropertyFloat (DDCChannelGroupHandle channelGroup, 
												const char* property,
												float *value);

int __stdcall DDC_GetChannelGroupPropertyDouble (DDCChannelGroupHandle channelGroup, 
												 const char* property,
												 double *value);

int __stdcall DDC_GetChannelGroupPropertyString (DDCChannelGroupHandle channelGroup, 
												 const char* property,
												 char value[], 
												 size_t valueSize);

int __stdcall DDC_CreateChannelPropertyUInt8 (DDCChannelHandle channel, 
											  const char *property,
											  unsigned char value);

int __stdcall DDC_CreateChannelPropertyInt16 (DDCChannelHandle channel, 
											  const char *property,
											  short value);

int __stdcall DDC_CreateChannelPropertyInt32 (DDCChannelHandle channel, 
											  const char *property,
											  long value);

int __stdcall DDC_CreateChannelPropertyFloat (DDCChannelHandle channel, 
											  const char *property,
											  float value);

int __stdcall DDC_CreateChannelPropertyDouble (DDCChannelHandle channel, 
											   const char *property,
											   double value);

int __stdcall DDC_CreateChannelPropertyString (DDCChannelHandle channel, 
											   const char *property,
											   const char *value);

int __stdcall DDC_SetChannelPropertyUInt8 (DDCChannelHandle channel, 
										   const char* property, 
										   unsigned char value);

int __stdcall DDC_SetChannelPropertyInt16 (DDCChannelHandle channel, 
										   const char* property, 
										   short value);

int __stdcall DDC_SetChannelPropertyInt32 (DDCChannelHandle channel, 
										   const char* property, 
										   long value);

int __stdcall DDC_SetChannelPropertyFloat (DDCChannelHandle channel, 
										   const char* property, 
										   float value);

int __stdcall DDC_SetChannelPropertyDouble (DDCChannelHandle channel, 
											const char* property, 
											double value);

int __stdcall DDC_SetChannelPropertyString (DDCChannelHandle channel, 
											const char* property, 
											const char *value);

int __stdcall DDC_GetChannelPropertyUInt8 (DDCChannelHandle channel, 
										   const char* property, 
										   unsigned char *value);

int __stdcall DDC_GetChannelPropertyInt16 (DDCChannelHandle channel, 
										   const char* property, 
										   short *value);

int __stdcall DDC_GetChannelPropertyInt32 (DDCChannelHandle channel, const 
										   char* property, 
										   long *value);

int __stdcall DDC_GetChannelPropertyFloat (DDCChannelHandle channel, 
										   const char* property, 
										   float *value);

int __stdcall DDC_GetChannelPropertyDouble (DDCChannelHandle channel, 
											const char* property, 
											double *value);

int __stdcall DDC_GetChannelPropertyString (DDCChannelHandle channel, 
											const char* property, 
											char value[], 
											size_t valueSize);

int __stdcall DDC_GetFilePropertyNameFromIndex (DDCFileHandle file, 
												size_t index, 
												char propertyName[], 
												size_t propertyNameSize);

int __stdcall DDC_GetFilePropertyNameLengthFromIndex (DDCFileHandle file, 
													  size_t index, 
													  size_t* propertyNameLength);

int __stdcall DDC_GetChannelGroupPropertyNameFromIndex (DDCChannelGroupHandle channelGroup, 
														size_t index, 
														char propertyName[], 
														size_t propertyNameSize);

int __stdcall DDC_GetChannelGroupPropertyNameLengthFromIndex (DDCChannelGroupHandle channelGroup, 
															  size_t index, 
															  size_t* propertyNameLength);

int __stdcall DDC_GetChannelPropertyNameFromIndex (DDCChannelHandle channel, 
												   size_t index, 
												   char propertyName[], 
												   size_t propertyNameSize);

int __stdcall DDC_GetChannelPropertyNameLengthFromIndex (DDCChannelHandle channel, 
														 size_t index, 
														 size_t* propertyNameLength);

//*****************************************************************************
/// -> Separate type-safe functions for non-C users
//*****************************************************************************

#ifdef __cplusplus
	}
#endif

#endif //!defined(_NILIBDDC_H)

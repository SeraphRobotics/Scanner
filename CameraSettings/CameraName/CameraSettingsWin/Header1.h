//https://social.msdn.microsoft.com/Forums/windowsdesktop/en-US/47b1317d-87e6-4121-9189-0defe1e2dd44/how-to-set-the-exposure-time-of-a-logitech-webcam?forum=windowsdirectshowdevelopment
HRESULT setExposureTime() {
	HRESULT hr;
	ICreateDevEnum *pSysDevEnum = NULL;

	hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL,
		CLSCTX_INPROC_SERVER, IID_ICreateDevEnum, (void **)&pSysDevEnum);

	if (FAILED(hr)) {
		return hr;
	}

	// Obtain a class enumerator for the video input device category
	IEnumMoniker *pEnumCat = NULL;
	hr = pSysDevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory, &pEnumCat, 0);

	if (hr == S_OK) {
		// Enumerate the monikers
		IMoniker *pMoniker = NULL;
		ULONG cFetched;
		while (pEnumCat->Next(1, &pMoniker, &cFetched) == S_OK) {
			IPropertyBag *pPropBag;
			hr = pMoniker->BindToStorage(0, 0, IID_IPropertyBag, (void **)&pPropBag);

			// To create an instance of the filter, do the following:
			IBaseFilter *pFilter;
			hr = pMoniker->BindToObject(NULL, NULL, IID_IBaseFilter, (void **)&pFilter);

			if (SUCCEEDED(hr)) {
				// To retrieve the filter's friendly name, do the following:
				VARIANT varName;
				VariantInit(&varName);
				hr = pPropBag->Read(L"FriendlyName", &varName, 0);
				if (SUCCEEDED(hr)){
					if (!strcmp(CString(varName.bstrVal), "Logitech HD Pro Webcam C920")) {
						// possible string problems with BSTR conversion stackoverflow

						// IKsPropertySet
						IKsPropertySet *pKsPropertySet;
						hr = pFilter->QueryInterface(IID_IKsPropertySet, (void **)&pKsPropertySet);
						if (SUCCEEDED(hr)) {




							KSPROPERTY_LP1_EXPOSURE_TIME_S expTime = { 0, 0 };
							DWORD dwReturned = 0;

							hr = pKsPropertySet->QuerySupported(PROPSETID_LOGITECH_PUBLIC1,
								KSPROPERTY_LP1_EXPOSURE_TIME,
								&dwReturned);

							hr = pKsPropertySet->Get(
								PROPSETID_LOGITECH_PUBLIC1,
								KSPROPERTY_LP1_EXPOSURE_TIME,
								NULL,
								0,
								&expTime,
								sizeof(expTime),
								&dwReturned);

							if (SUCCEEDED(hr)){
								expTime.ulExposureTime = 100;
								expTime.Header.ulFlags = KSPROPERTY_CAMERACONTROL_FLAGS_MANUAL;

								/**
								hr = pKsPropertySet->Set(
								PROPSETID_LOGITECH_PUBLIC1,
								KSPROPERTY_LP1_EXPOSURE_TIME,
								INSTANCEDATA_OF_PROPERTY_PTR(&expTime),
								INSTANCEDATA_OF_PROPERTY_SIZE(expTime),
								&expTime,
								sizeof(expTime));
								**/

								// Second try caused by a hint from logitech team
								// throws another failure
								hr = pKsPropertySet->Set(
									PROPSETID_LOGITECH_PUBLIC1,
									KSPROPERTY_LP1_EXPOSURE_TIME,
									NULL,
									0,
									&expTime,
									sizeof(expTime));

								hr = pKsPropertySet->Get(
									PROPSETID_LOGITECH_PUBLIC1,
									KSPROPERTY_LP1_EXPOSURE_TIME,
									NULL,
									0,
									&expTime,
									sizeof(expTime),
									&dwReturned);

								int i = 10;
							}

						}
						pKsPropertySet->Release();
					}
				}
				VariantClear(&varName);


				// Now add the filter to the graph.
				// Remember to release pFilter later.
				pPropBag->Release();
			}
			pMoniker->Release();
		}
		pEnumCat->Release();
	}
	pSysDevEnum->Release();
	return hr;
}
/*
 * @Author: Libra
 * @Date: 2025-05-03 02:39:33
 * @LastEditors: Libra
 * @Description: 
 */
#include "components/IME/IMESelectorWidget.h"
#include <QComboBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QString>
#include <vector>
#include <QLineEdit>
#include <windows.h>
#include <imm.h>
#include <msctf.h>
#include <initguid.h>
#include "components/base/Button.h"
#include "components/base/Input.h"
#include "components/base/Select.h"

enum IMEType { TSF_IME, IMM_IME };

struct IMEItem {
    IMEType  type;
    CLSID    clsid;
    GUID     profileGuid;
    LANGID   langid;
    HKL      hkl;
    QString  name;
};

class IMESelectorWidgetPrivate {
public:
    std::vector<IMEItem> imeList;
    QComboBox* comboBox = nullptr;

    // 枚举 TSF
    void LoadTSF() {
        if (FAILED(CoInitializeEx(NULL, COINIT_APARTMENTTHREADED)))
            return;
        ITfInputProcessorProfiles* pProfiles = nullptr;
        if (SUCCEEDED(CoCreateInstance(
                CLSID_TF_InputProcessorProfiles, nullptr,
                CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pProfiles))))
        {
            LANGID* langs = nullptr;
            ULONG  count = 0;
            if (SUCCEEDED(pProfiles->GetLanguageList(&langs, &count))) {
                for (ULONG i = 0; i < count; ++i) {
                    LANGID lid = langs[i];
                    IEnumTfLanguageProfiles* pEnum = nullptr;
                    if (SUCCEEDED(pProfiles->EnumLanguageProfiles(lid, &pEnum))) {
                        TF_LANGUAGEPROFILE prof;
                        ULONG fetched = 0;
                        while (pEnum->Next(1, &prof, &fetched) == S_OK) {
                            if (!IsEqualGUID(prof.catid, GUID_TFCAT_TIP_KEYBOARD))
                                continue;
                            wchar_t* desc = nullptr;
                            if (SUCCEEDED(pProfiles->GetLanguageProfileDescription(
                                    prof.clsid, lid, prof.guidProfile, &desc)))
                            {
                                IMEItem it;
                                it.type        = TSF_IME;
                                it.clsid       = prof.clsid;
                                it.profileGuid = prof.guidProfile;
                                it.langid      = lid;
                                it.name        = QString::fromWCharArray(desc)
                                                 + QStringLiteral(" (TSF)");
                                imeList.push_back(it);
                                CoTaskMemFree(desc);
                            }
                        }
                        pEnum->Release();
                    }
                }
                CoTaskMemFree(langs);
            }
            pProfiles->Release();
        }
        CoUninitialize();
    }

    // 枚举 IMM
    void LoadIMM() {
        int n = GetKeyboardLayoutList(0, nullptr);
        if (n <= 0) return;
        std::vector<HKL> layouts(n);
        GetKeyboardLayoutList(n, layouts.data());
        for (auto hkl : layouts) {
            UINT len = ImmGetDescriptionW(hkl, nullptr, 0);
            if (len == 0) continue;
            std::wstring buf(len + 1, L'\0');
            ImmGetDescriptionW(hkl, &buf[0], len + 1);
            IMEItem it;
            it.type = IMM_IME;
            it.hkl  = hkl;
            it.name = QString::fromWCharArray(buf.c_str())
                      + QStringLiteral(" (IMM)");
            imeList.push_back(it);
        }
    }
};

IMESelectorWidget::IMESelectorWidget(QWidget* parent)
    : QWidget(parent)
{
    auto d = new IMESelectorWidgetPrivate;
    auto* comboBox     = new Select;
    auto* switchButton = new Button("切换输入法");
    auto* testEdit     = new Input;
    testEdit->setPlaceholderText("在这里测试输入法切换效果…");
    auto* layout       = new QVBoxLayout(this);
    layout->addWidget(comboBox);
    layout->addWidget(switchButton);
    layout->addWidget(testEdit);
    d->comboBox = comboBox;
    d->LoadTSF();
    d->LoadIMM();

    for (const auto& it : d->imeList) {
        comboBox->addOption(it.name);
    }

    connect(switchButton, &QPushButton::clicked, this, [=]() {
        int idx = comboBox->currentIndex();
        if (idx < 0 || idx >= static_cast<int>(d->imeList.size())) return;
        const IMEItem& item = d->imeList[idx];
        bool success = false;
        if (item.type == TSF_IME) {
            if (SUCCEEDED(CoInitializeEx(NULL, COINIT_APARTMENTTHREADED))) {
                ITfInputProcessorProfiles* pProf = nullptr;
                if (SUCCEEDED(CoCreateInstance(
                        CLSID_TF_InputProcessorProfiles, nullptr,
                        CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pProf))))
                {
                    HRESULT hr = pProf->ActivateLanguageProfile(
                        item.clsid, item.langid, item.profileGuid);
                    success = SUCCEEDED(hr);
                    pProf->Release();
                }
                CoUninitialize();
            }
        } else {
            HKL ret = ActivateKeyboardLayout(
                item.hkl, KLF_SETFORPROCESS | KLF_REORDER);
            success = (ret != NULL);
        }
        if (success)
            QMessageBox::information(
                this, QStringLiteral("切换成功"),
                QStringLiteral("已切换到：%1").arg(item.name));
        else
            QMessageBox::warning(
                this, QStringLiteral("切换失败"),
                QStringLiteral("无法切换到：%1").arg(item.name));
    });
}

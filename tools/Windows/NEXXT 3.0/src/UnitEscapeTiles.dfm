object FormEscapeTiles: TFormEscapeTiles
  Left = 0
  Top = 0
  BorderStyle = bsToolWindow
  Caption = 'Tileswitches (mmc2, mmc4, neomappers)'
  ClientHeight = 428
  ClientWidth = 364
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  KeyPreview = True
  OldCreateOrder = False
  OnCreate = FormCreate
  OnKeyDown = FormKeyDown
  OnKeyPress = FormKeyPress
  OnKeyUp = FormKeyUp
  PixelsPerInch = 96
  TextHeight = 13
  object Label5: TLabel
    Left = 7
    Top = 370
    Width = 237
    Height = 52
    Caption = 
      'Note 3: For neomappers, using tile $ff for switching sprites wil' +
      'l pose a problem on lines with undrawn sprites. This includes $f' +
      'e in 8x16 mode. See secondary OAM on the nesdev wiki.'
    WordWrap = True
  end
  object btnModeOnOff: TSpeedButton
    Left = 268
    Top = 388
    Width = 90
    Height = 34
    AllowAllUp = True
    GroupIndex = 2
    Caption = 'mode off'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    OnClick = btnModeOnOffClick
  end
  object Label6: TLabel
    Left = 7
    Top = 324
    Width = 294
    Height = 13
    Caption = 'Note 1: 8x16 mode + switching is not supported at this time. '
    WordWrap = True
  end
  object Label7: TLabel
    Left = 7
    Top = 340
    Width = 244
    Height = 26
    Caption = 
      'Note 2: Known bug - using more than 1 switch in a metasprite exh' +
      'ibits inaccurate behaviour.'
    WordWrap = True
  end
  object GroupBox1: TGroupBox
    Left = 5
    Top = 0
    Width = 73
    Height = 110
    Caption = 'Presets'
    TabOrder = 0
    object btnMMC2: TSpeedButton
      Left = 5
      Top = 16
      Width = 62
      Height = 20
      GroupIndex = 1
      Down = True
      Caption = 'MMC2/4'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -10
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      OnClick = btnMMC2Click
    end
    object btnPreset1: TSpeedButton
      Tag = 1
      Left = 5
      Top = 38
      Width = 62
      Height = 20
      GroupIndex = 1
      Caption = 'Preset 1'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -10
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      OnClick = btnMMC2Click
    end
    object btnPreset2: TSpeedButton
      Tag = 2
      Left = 5
      Top = 60
      Width = 62
      Height = 20
      GroupIndex = 1
      Caption = 'Preset 2'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -10
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      OnClick = btnMMC2Click
    end
    object btnStore: TSpeedButton
      Left = 5
      Top = 82
      Width = 62
      Height = 20
      Caption = 'Store preset'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -10
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      OnClick = btnStoreClick
    end
  end
  object GroupBox2: TGroupBox
    Left = 82
    Top = 0
    Width = 278
    Height = 110
    Caption = 'Tileswitch behaviour'
    TabOrder = 1
    object Label1: TLabel
      Left = 122
      Top = 19
      Width = 54
      Height = 13
      Caption = 'switches to'
    end
    object Label2: TLabel
      Left = 122
      Top = 42
      Width = 54
      Height = 13
      Caption = 'switches to'
    end
    object Label3: TLabel
      Left = 122
      Top = 65
      Width = 54
      Height = 13
      Caption = 'switches to'
    end
    object Label4: TLabel
      Left = 122
      Top = 88
      Width = 54
      Height = 13
      Caption = 'switches to'
    end
    object chkTile1: TCheckBox
      Left = 6
      Top = 19
      Width = 44
      Height = 17
      TabStop = False
      Caption = 'tile $'
      Checked = True
      State = cbChecked
      TabOrder = 0
      OnClick = chkTile1Click
    end
    object chkTile2: TCheckBox
      Tag = 1
      Left = 6
      Top = 42
      Width = 44
      Height = 17
      TabStop = False
      Caption = 'tile $'
      Checked = True
      State = cbChecked
      TabOrder = 1
      OnClick = chkTile1Click
    end
    object chkTile3: TCheckBox
      Tag = 2
      Left = 6
      Top = 65
      Width = 44
      Height = 17
      TabStop = False
      Caption = 'tile $'
      TabOrder = 2
      OnClick = chkTile1Click
    end
    object chkTile4: TCheckBox
      Tag = 3
      Left = 6
      Top = 88
      Width = 44
      Height = 17
      TabStop = False
      Caption = 'tile $'
      TabOrder = 3
      OnClick = chkTile1Click
    end
    object ComboBox1: TComboBox
      Left = 54
      Top = 15
      Width = 62
      Height = 21
      Style = csDropDownList
      ItemHeight = 0
      TabOrder = 4
      TabStop = False
      OnChange = ComboBox1Change
    end
    object ComboBox2: TComboBox
      Left = 54
      Top = 38
      Width = 62
      Height = 21
      Style = csDropDownList
      ItemHeight = 0
      TabOrder = 5
      TabStop = False
      OnChange = ComboBox1Change
    end
    object ComboBox3: TComboBox
      Left = 54
      Top = 61
      Width = 62
      Height = 21
      Style = csDropDownList
      ItemHeight = 0
      TabOrder = 6
      TabStop = False
      OnChange = ComboBox1Change
    end
    object ComboBox4: TComboBox
      Left = 54
      Top = 84
      Width = 62
      Height = 21
      Style = csDropDownList
      ItemHeight = 0
      TabOrder = 7
      TabStop = False
      OnChange = ComboBox1Change
    end
    object ComboBox5: TComboBox
      Left = 182
      Top = 15
      Width = 90
      Height = 21
      Style = csDropDownList
      ItemHeight = 0
      TabOrder = 8
      TabStop = False
      OnChange = ComboBox5Change
    end
    object ComboBox6: TComboBox
      Left = 182
      Top = 38
      Width = 90
      Height = 21
      Style = csDropDownList
      ItemHeight = 0
      TabOrder = 9
      TabStop = False
      OnChange = ComboBox5Change
    end
    object ComboBox7: TComboBox
      Left = 182
      Top = 61
      Width = 90
      Height = 21
      Style = csDropDownList
      ItemHeight = 0
      TabOrder = 10
      TabStop = False
      OnChange = ComboBox5Change
    end
    object ComboBox8: TComboBox
      Left = 182
      Top = 84
      Width = 90
      Height = 21
      Style = csDropDownList
      ItemHeight = 0
      TabOrder = 11
      TabStop = False
      OnChange = ComboBox5Change
    end
  end
  object GroupBox3: TGroupBox
    Left = 5
    Top = 109
    Width = 355
    Height = 58
    Caption = 'General worskpace behaviour'
    TabOrder = 2
    object chkDoublePair: TCheckBox
      Left = 5
      Top = 17
      Width = 330
      Height = 17
      TabStop = False
      Caption = 'Treat 2 checked, 2 unchecked as toggleable double pair sets'
      Checked = True
      State = cbChecked
      TabOrder = 0
      OnClick = chkDoublePairClick
    end
    object chkBG: TCheckBox
      Left = 5
      Top = 36
      Width = 180
      Height = 17
      TabStop = False
      Caption = 'Affect backgrounds && metatiles'
      Checked = True
      State = cbChecked
      TabOrder = 1
      OnClick = chkBGClick
    end
    object chkSpr: TCheckBox
      Left = 193
      Top = 36
      Width = 156
      Height = 17
      TabStop = False
      Caption = 'Affect metasprites (beta)'
      Checked = True
      State = cbChecked
      TabOrder = 2
      OnClick = chkSprClick
    end
  end
  object GroupBox4: TGroupBox
    Left = 5
    Top = 166
    Width = 109
    Height = 40
    Caption = 'Double pairs start in'
    TabOrder = 3
    object RadioDoublePair1st: TRadioButton
      Left = 5
      Top = 19
      Width = 56
      Height = 17
      Caption = '1st set'
      Checked = True
      TabOrder = 0
      TabStop = True
      OnClick = chkDoublePairClick
    end
    object RadioDoublePair2nd: TRadioButton
      Left = 61
      Top = 19
      Width = 44
      Height = 17
      Caption = '2:nd'
      TabOrder = 1
      OnClick = chkDoublePairClick
    end
  end
  object GroupBox5: TGroupBox
    Left = 116
    Top = 166
    Width = 244
    Height = 40
    Caption = 'Starting set; otherwise'
    TabOrder = 4
    object RadioUse1stChecked: TRadioButton
      Left = 5
      Top = 17
      Width = 80
      Height = 17
      Caption = '1st checked'
      Checked = True
      TabOrder = 0
      TabStop = True
      OnClick = chkDoublePairClick
    end
    object RadioUseActive: TRadioButton
      Left = 190
      Top = 17
      Width = 50
      Height = 17
      Caption = 'active'
      TabOrder = 1
      OnClick = chkDoublePairClick
    end
    object RadioUseA: TRadioButton
      Left = 82
      Top = 17
      Width = 28
      Height = 17
      Caption = 'A'
      TabOrder = 2
      OnClick = chkDoublePairClick
    end
    object RadioUseB: TRadioButton
      Left = 108
      Top = 17
      Width = 28
      Height = 17
      Caption = 'B'
      TabOrder = 3
      OnClick = chkDoublePairClick
    end
    object RadioUseC: TRadioButton
      Left = 134
      Top = 17
      Width = 28
      Height = 17
      Caption = 'C'
      TabOrder = 4
      OnClick = chkDoublePairClick
    end
    object RadioUseD: TRadioButton
      Left = 162
      Top = 17
      Width = 28
      Height = 17
      Caption = 'D'
      TabOrder = 5
      OnClick = chkDoublePairClick
    end
  end
  object GroupBox6: TGroupBox
    Left = 5
    Top = 205
    Width = 355
    Height = 56
    Caption = 'Metatile editor - each metatile is shown starting as'
    TabOrder = 5
    object Label8: TLabel
      Left = 8
      Top = 36
      Width = 322
      Height = 13
      Caption = 
        'Options within { } are overruled if the "double pair" rule is in' +
        ' effect.'
      WordWrap = True
    end
    object RadioUse1stChecked_mt: TRadioButton
      Left = 5
      Top = 17
      Width = 84
      Height = 17
      Caption = '{1st checked'
      TabOrder = 0
      OnClick = chkDoublePairClick
    end
    object RadioUseActive_mt: TRadioButton
      Left = 199
      Top = 17
      Width = 50
      Height = 17
      Caption = 'active'
      Checked = True
      TabOrder = 1
      TabStop = True
      OnClick = chkDoublePairClick
    end
    object RadioUseA_mt: TRadioButton
      Left = 86
      Top = 17
      Width = 28
      Height = 17
      Caption = 'A'
      TabOrder = 2
      OnClick = chkDoublePairClick
    end
    object RadioUseB_mt: TRadioButton
      Left = 112
      Top = 17
      Width = 28
      Height = 17
      Caption = 'B'
      TabOrder = 3
      OnClick = chkDoublePairClick
    end
    object RadioUseC_mt: TRadioButton
      Left = 138
      Top = 17
      Width = 28
      Height = 17
      Caption = 'C'
      TabOrder = 4
      OnClick = chkDoublePairClick
    end
    object RadioUseD_mt: TRadioButton
      Left = 166
      Top = 17
      Width = 30
      Height = 17
      Caption = 'D}'
      TabOrder = 5
      OnClick = chkDoublePairClick
    end
    object RadioUseSame_mt: TRadioButton
      Left = 249
      Top = 17
      Width = 104
      Height = 17
      Caption = 'same as previous'
      TabOrder = 6
      OnClick = chkDoublePairClick
    end
  end
  object GroupBox7: TGroupBox
    Left = 5
    Top = 260
    Width = 355
    Height = 57
    Caption = 'Sprite switch detection'
    TabOrder = 6
    object Radio1stLine: TRadioButton
      Left = 5
      Top = 17
      Width = 150
      Height = 17
      Caption = 'After 1st line of tile (mmc2)'
      Checked = True
      TabOrder = 0
      TabStop = True
      OnClick = chkDoublePairClick
    end
    object RadioEveryLine: TRadioButton
      Left = 170
      Top = 17
      Width = 136
      Height = 17
      Caption = 'After every line (mmc4)'
      Enabled = False
      TabOrder = 1
      OnClick = chkDoublePairClick
    end
    object RadioLastLine: TRadioButton
      Left = 5
      Top = 36
      Width = 235
      Height = 17
      Caption = 'After end of tile (possibly for neomappers)'
      Enabled = False
      TabOrder = 2
      OnClick = chkDoublePairClick
    end
  end
end

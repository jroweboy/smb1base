object FormPipelineHelper: TFormPipelineHelper
  Left = 0
  Top = 0
  BorderStyle = bsToolWindow
  Caption = 'Pipelines Assistant'
  ClientHeight = 143
  ClientWidth = 428
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  PopupMode = pmExplicit
  OnPaint = FormPaint
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object GroupBox1: TGroupBox
    Left = 2
    Top = 2
    Width = 423
    Height = 46
    Caption = 'Associate metatiles from:'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 0
    object SpeedButton1: TSpeedButton
      Left = 3
      Top = 12
      Width = 48
      Height = 16
      Caption = 'choose...'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -9
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      OnClick = SpeedButton1Click
      OnMouseLeave = CheckBox1MouseLeave
    end
    object SpeedButton2: TSpeedButton
      Left = 54
      Top = 12
      Width = 32
      Height = 16
      Caption = 'clear'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -9
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      OnClick = SpeedButton2Click
      OnMouseLeave = CheckBox1MouseLeave
    end
    object Label1: TLabel
      Left = 4
      Top = 30
      Width = 64
      Height = 11
      Caption = 'No path chosen'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -9
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object CheckBox1: TCheckBox
      Left = 100
      Top = 12
      Width = 63
      Height = 17
      Caption = 'associated'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -9
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 0
      OnClick = CheckBox1Click
      OnMouseEnter = CheckBox1MouseEnter
      OnMouseLeave = CheckBox1MouseLeave
      OnMouseUp = CheckBox1MouseUp
    end
    object CheckBox3: TCheckBox
      Left = 290
      Top = 12
      Width = 126
      Height = 17
      Caption = 'view / edit (linked mode)'
      Enabled = False
      TabOrder = 1
      Visible = False
      OnMouseLeave = CheckBox1MouseLeave
    end
  end
  object GroupBox2: TGroupBox
    Left = 2
    Top = 48
    Width = 423
    Height = 46
    Caption = 'Associate tileset from:'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 1
    object SpeedButton3: TSpeedButton
      Tag = 1
      Left = 3
      Top = 12
      Width = 48
      Height = 16
      Caption = 'choose...'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -9
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      OnClick = SpeedButton1Click
      OnMouseLeave = CheckBox1MouseLeave
    end
    object SpeedButton4: TSpeedButton
      Left = 54
      Top = 12
      Width = 32
      Height = 16
      Caption = 'clear'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -9
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      OnClick = SpeedButton4Click
      OnMouseLeave = CheckBox1MouseLeave
    end
    object Label2: TLabel
      Left = 4
      Top = 30
      Width = 64
      Height = 11
      Caption = 'No path chosen'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -9
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object btnRefreshCHRlink: TSpeedButton
      Left = 330
      Top = 12
      Width = 86
      Height = 16
      Caption = 'refresh from source'
      Enabled = False
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -9
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      OnClick = btnRefreshCHRlinkClick
      OnMouseEnter = btnRefreshCHRlinkMouseEnter
      OnMouseLeave = CheckBox1MouseLeave
    end
    object CheckBox2: TCheckBox
      Left = 100
      Top = 12
      Width = 63
      Height = 17
      Caption = 'associated'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -9
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 0
      OnClick = CheckBox2Click
      OnMouseEnter = CheckBox2MouseEnter
      OnMouseLeave = CheckBox1MouseLeave
      OnMouseUp = CheckBox2MouseUp
    end
    object CheckBox4: TCheckBox
      Left = 200
      Top = 12
      Width = 126
      Height = 17
      Caption = 'view / edit (linked mode)'
      TabOrder = 1
      OnClick = CheckBox4Click
      OnMouseEnter = CheckBox4MouseEnter
      OnMouseLeave = CheckBox1MouseLeave
    end
  end
  object GroupBox3: TGroupBox
    Left = 2
    Top = 94
    Width = 289
    Height = 47
    Caption = 'When saving session, auto export...'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 2
    object chkAutoExpMetatilesBMP: TCheckBox
      Left = 8
      Top = 12
      Width = 128
      Height = 17
      Caption = 'metatiles as image'
      TabOrder = 0
      OnMouseEnter = chkAutoExpMetatilesBMPMouseEnter
      OnMouseLeave = CheckBox1MouseLeave
    end
  end
  object GroupBox4: TGroupBox
    Left = 240
    Top = 94
    Width = 185
    Height = 47
    Caption = '...to:'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 3
    object btnType: TSpeedButton
      Left = 84
      Top = 26
      Width = 37
      Height = 17
      GroupIndex = 1
      Down = True
      Caption = 'filetype'
      OnMouseEnter = btnTypeMouseEnter
      OnMouseLeave = CheckBox1MouseLeave
    end
    object btnName: TSpeedButton
      Left = 122
      Top = 26
      Width = 28
      Height = 17
      GroupIndex = 1
      Caption = 'title'
      OnMouseEnter = btnNameMouseEnter
      OnMouseLeave = CheckBox1MouseLeave
    end
    object btnAsset: TSpeedButton
      Left = 151
      Top = 26
      Width = 30
      Height = 17
      GroupIndex = 1
      Caption = 'asset'
      OnMouseEnter = btnAssetMouseEnter
      OnMouseLeave = CheckBox1MouseLeave
    end
    object RadioButton1: TRadioButton
      Left = 5
      Top = 11
      Width = 69
      Height = 15
      Caption = 'same folder'
      TabOrder = 0
      OnMouseEnter = RadioButton1MouseEnter
      OnMouseLeave = CheckBox1MouseLeave
    end
    object RadioButton2: TRadioButton
      Left = 5
      Top = 29
      Width = 79
      Height = 15
      Caption = 'subfolder by...'
      Checked = True
      TabOrder = 1
      TabStop = True
      OnMouseEnter = RadioButton2MouseEnter
      OnMouseLeave = CheckBox1MouseLeave
    end
    object RadioButton3: TRadioButton
      Left = 76
      Top = 11
      Width = 104
      Height = 15
      Caption = '\...\nexxt\autoexports'
      TabOrder = 2
      OnMouseEnter = RadioButton3MouseEnter
      OnMouseLeave = CheckBox1MouseLeave
    end
  end
  object OpenDialogPath: TOpenDialog
    DefaultExt = 'nss'
    Filter = 'NES Screen Tool Session (*.nss)|*.nss|Any file (*.*)|*.*'
    Title = 'Associate file'
    Left = 315
    Top = 2
  end
end

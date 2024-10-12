object FormSwapBanks: TFormSwapBanks
  Left = 0
  Top = 0
  BorderStyle = bsToolWindow
  Caption = 'CHR Bank Swapper'
  ClientHeight = 409
  ClientWidth = 338
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  OnCreate = FormCreate
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Image1: TImage
    Left = 5
    Top = 8
    Width = 128
    Height = 128
    OnMouseDown = Image1MouseDown
    OnMouseEnter = Image1MouseEnter
    OnMouseLeave = Image1MouseLeave
    OnMouseMove = Image1MouseMove
  end
  object Image2: TImage
    Left = 206
    Top = 8
    Width = 128
    Height = 128
    OnMouseDown = Image2MouseDown
    OnMouseEnter = Image2MouseEnter
    OnMouseLeave = Image2MouseLeave
    OnMouseMove = Image2MouseMove
  end
  object ListBox1: TListBox
    Left = 5
    Top = 142
    Width = 161
    Height = 264
    ItemHeight = 13
    TabOrder = 0
    OnClick = ListBox1Click
  end
  object ListBox2: TListBox
    Left = 172
    Top = 142
    Width = 162
    Height = 264
    ItemHeight = 13
    MultiSelect = True
    TabOrder = 1
    OnClick = ListBox2Click
  end
  object chkInclLabel: TCheckBox
    Left = 136
    Top = 106
    Width = 68
    Height = 17
    Caption = 'incl. label'
    Checked = True
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -10
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    State = cbChecked
    TabOrder = 2
  end
  object Button2: TButton
    Left = 135
    Top = 8
    Width = 70
    Height = 21
    Caption = 'Deselect'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -10
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 3
    OnClick = Button2Click
  end
  object chkInclProps: TCheckBox
    Left = 136
    Top = 122
    Width = 68
    Height = 17
    Caption = 'incl. props'
    Checked = True
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -10
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    State = cbChecked
    TabOrder = 4
  end
  object btnSwap: TButton
    Left = 135
    Top = 30
    Width = 70
    Height = 21
    Caption = '< Swap >'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -10
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 5
    OnClick = btnSwapClick
  end
  object btnClone: TButton
    Left = 135
    Top = 52
    Width = 70
    Height = 21
    Caption = '  Clone >'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -10
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 6
    OnClick = btnCloneClick
  end
  object btnMove: TButton
    Left = 135
    Top = 74
    Width = 70
    Height = 21
    Caption = '  Move >'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -10
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 7
    OnClick = btnMoveClick
  end
end
